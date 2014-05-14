#ifndef _EVENT_PIE_H
#define _EVENT_PIE_H

#include "Config.h"

#include "Timer.h"
#include "Task.h"

#include "Util.h"
#include "Error.h"

#include "Ev/Ev.h"

namespace EventPie{
    
    int run();
    void quit();
    
    void addTimer(Timer *timer);
    void removeTimer(Timer *timer);
    
    void defer(Task task);
    void deferAsync(Task task);
};

#endif //_EVENT_PIE_H
