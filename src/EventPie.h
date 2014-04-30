#ifndef _EVENT_PIE_H
#define _EVENT_PIE_H

#include "Timer.h"
#include "Task.h"

#include "Util.h"

namespace EventPie{
    
    int run();
    void stop();
    
    void addTimer(Timer *timer);
    void removeTimer(Timer *timer);
    
    void defer(Task task);
    void deferAsync(Task task);
};

#endif //_EVENT_PIE_H
