#ifndef _THREAD_POOL_H
#define _THREAD_POOL_H

#include "Task.h"

#include <thread>
#include <mutex>
#include <condition_variable>

#include <functional>

#include <vector>
#include <queue>

#define THREAD_POOL_DEFAULT_SIZE 20

namespace EventPie{
    
    class ThreadPool{
    public:
        
        ThreadPool();
        ThreadPool(int nThreads);
        virtual ~ThreadPool();
        
        void enqueue(Task job);
        
        void kill();
        
    protected:
        void worker();
        void createThreads(int nThreads);
        
    protected:
        std::vector<std::thread> threads;
        
        std::queue<Task> tasks;
        std::condition_variable signal;
        std::mutex queueMutex;
        
        bool running;
    };
};

#endif //_THREAD_POOL_H
