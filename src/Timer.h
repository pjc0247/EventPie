#ifndef _TIMER_H
#define _TIMER_H

#include <functional>

namespace EventPie{
    
    typedef std::function<void()> TimerCallback;
    
    class Timer{
    public:
        Timer();
        Timer(TimerCallback callback, int interval);
        virtual ~Timer();
        
        void step(int dt);
        void reset();
        void stop();
        
        int getInterval() const;
        int getTicks() const;
        
    protected:
        int interval;
        TimerCallback callback;
        
        int ticks;
    };
};

#endif //_TIMER_H
