#include "EventPie.h"

#include "Reaction.h"
#include "Timer.h"
#include "ThreadPool.h"

#include <queue>
#include <vector>
#include <chrono>
#include <mutex>

using namespace std;
using namespace std::chrono;

namespace EventPie{
    
    static vector<Timer*> timers;
    static queue<Task> tasks;
    
    static mutex taskQueueMutex;
    
    static ThreadPool threadPool;
    
    static bool running;
    
    void processTasks(){
        /* process task queue */
        unique_lock<mutex> lock( taskQueueMutex );
        while( !tasks.empty() ){
            Task task = tasks.front();
                task();
            tasks.pop();
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
        running = true;
        
        auto timePoint = system_clock::now();
        
        while( running ){
            processTasks();
            if( !running ) break;
            
            /* calculate loop time */
            auto loopTime =
                duration_cast<milliseconds>(system_clock::now() - timePoint).count();
            
            if( loopTime ){
                timePoint = system_clock::now();
            
                processTimers( (int)loopTime );
            }
        }
        return 0;
    }
    void stop(){
        running = false;
    }
    
    void addTimer(Timer *timer){
        timers.push_back( timer );
    }
    void removeTimer(Timer *timer){
        /* fast-remove */
        for(int i=0;i<timers.size();i++){
            if( timers[i] == timer ){
                std::swap( timers[i], timers.back() );
                timers.pop_back();
            }
        }
    }
    
    void addTask(Task task){
        unique_lock<mutex> lock( taskQueueMutex );
            tasks.push( task );
    }
    
    void defer(Task task){
        threadPool.enqueue( task );
    }
}
