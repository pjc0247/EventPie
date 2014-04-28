#ifndef _EVENT_PIE_H
#define _EVENT_PIE_H

#include "Task.h"

namespace EventPie{
    class Timer;
    class Reaction;
    
    int run();
    void stop();
    
    void addTimer(Timer *timer);
    void removeTimer(Timer *timer);
    
    void addTask(Task task);
    
    void defer(Task task);
};

#endif //_EVENT_PIE_H
