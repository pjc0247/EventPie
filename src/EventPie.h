#ifndef _EVENT_PIE_H
#define _EVENT_PIE_H

namespace EventPie{
    
    class Timer;
    class Event;
    
    int run();
    
    void addTimer(Timer *timer);
    void removeTimer(Timer *timer);
    
};

#endif //_EVENT_PIE_H
