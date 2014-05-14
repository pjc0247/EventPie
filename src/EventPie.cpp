#include "EventPie.h"

#include "ThreadPool.h"
#include "Ev/Ev.h"
#include "Io/Interface.h"

#include <queue>
#include <vector>
#include <chrono>
#include <mutex>

using namespace std;
using namespace std::chrono;
using namespace EventPie::Config;

/*
#define OUT_Q tasks[taskQueueSwitch.load()]
#define IN_Q tasks[1-taskQueueSwitch.load()]
#define OUT_LOCK taskQueueMutex[taskQueueSwitch.load()]
#define IN_LOCK taskQueueMutex[1-taskQueueSwitch.load()]
#define SWITCH_Q taskQueueSwitch ^= 1;
*/

namespace EventPie{
    
    static vector<Timer*> timers;
    static queue<Task> tasks[2];
    static int taskQueueSwitch = 0;
    
    static mutex taskQueueMutex[2];
   
    static ThreadPool threadPool( threadPoolSize );
    
    static bool running;
    
    int pd = 0;
    
    int getPd(){ return pd; };
    
    void processEvents(){
        Ev::Event *events;

        int nEvents = Ev::poll( &events );
        for(int i=0;i<nEvents;i++){
            Io::Interface *interface =
                (Io::Interface*)events[i].userData;
            
            /* invoke io callback */
            switch( events[i].filter ){
                case Ev::eRead:
                    interface->onRead();
                    break;
                case Ev::eWrite:
                    interface->onWritten();
                    break;
            }
        }
    }
    void processTasks(){
        /* process task queue */
        std::atomic_thread_fence( std::memory_order_acquire );
        int idx = taskQueueSwitch;
        
        unique_lock<mutex> lock( taskQueueMutex[idx] );
        
        while( !tasks[idx].empty() ){
            Task task = tasks[idx].front();
            tasks[idx].pop();
                task();
        }
        lock.unlock();
    }
    void processTimers(int dt){
        /* process timers */
        for( auto &timer : timers ){
            timer->step( dt );
        }
    }
    
    int run(){
        if( Ev::initialize() == false )
            return eEvInitializeError;
        
        running = true;
        
        auto timePoint = system_clock::now();
        int loopCount = 0;
        
        while( running ){
            /* poll ev */
            processEvents();
            if( !running ) break;
            
            /* process tasks */
            processTasks();
            if( !running ) break;
            
            /* calculate loop time */
            auto loopTime =
                duration_cast<milliseconds>(system_clock::now() - timePoint).count();
            
            if( loopTime ){
                timePoint = system_clock::now();
            
                processTimers( (int)loopTime );
            }
            
            taskQueueSwitch ^= 1;
            std::atomic_thread_fence( std::memory_order_release );
        }
        
        Ev::quit();
        
        return 0;
    }
    void quit(){
        running = false;
    }
    
    void addTimer(Timer *timer){
        timers.push_back( timer );
    }
    void removeTimer(Timer *timer){
        /* fast-remove */
        for(int i=0;i<timers.size();i++){
            if( timers[i] == timer ){
                timers[i] = timers.back();
                timers.pop_back();
                break;
            }
        }
    }
    
    void defer(Task task){
        std::atomic_thread_fence( std::memory_order_acquire );
        int idx = taskQueueSwitch;
        
        unique_lock<mutex> lock( taskQueueMutex[idx] );
            tasks[idx].push( task );
        lock.unlock();
    }
    
    void deferAsync(Task task){
        threadPool.enqueue( task );
    }
}
