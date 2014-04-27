#include "Timer.h"

#include "EventPie.h"

#include <assert.h>

namespace EventPie{
  
    Timer::Timer() :
        callback(nullptr),
        interval(0), ticks(0) {
        
    }
    Timer::Timer(TimerCallback _callback, int _interval) :
        callback(_callback),
        interval(_interval), ticks(0){
        
        addTimer( this );
    }
    Timer::~Timer(){
        
    }
    
    void Timer::step(int dt){
        assert( dt >= 0 );
        
        ticks += dt;
        
        if( ticks >= interval ){
            callback();
            
            reset();
        }
    }
    void Timer::reset(){
        ticks = 0;
    }
    void Timer::stop(){
        removeTimer( this );
    }
    
    int Timer::getInterval() const{
        return interval;
    }
    int Timer::getTicks() const{
        return ticks;
    }
};
