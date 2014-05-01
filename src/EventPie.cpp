#include "EventPie.h"

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
    
    static ThreadPool threadPool( cfgThreadPoolSize );
    
    static bool running;
    
    void processTasks(){
        /* process task queue */
        unique_lock<mutex> lock( taskQueueMutex );
        while( !tasks.empty() ){
            Task task = tasks.front();
            tasks.pop();
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
                timers[i] = timers.back();
                timers.pop_back();
                break;
            }
        }
    }
    
    void defer(Task task){
        unique_lock<mutex> lock( taskQueueMutex );
            tasks.push( task );
    }
    
    void deferAsync(Task task){
        threadPool.enqueue( task );
    }
}
