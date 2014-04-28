#ifndef _TASK_H
#define _TASK_H

#include <functional>

namespace EventPie{
    typedef std::function<void()> Task;
};

#endif //_TASK_H
