#ifndef _THREAD_POOL_H
#define _THREAD_POOL_H

#include <thread>
#include <mutex>
#include <condition_variable>

#include <functional>

#include <vector>
#include <queue>

namespace EventPie{
    
    class ThreadPool{
    public:
        typedef std::function<void()> Job;
        
        ThreadPool();
        ThreadPool(int nThreads);
        virtual ~ThreadPool();
        
        void enqueue(Job job);
        
        void kill();
        
    protected:
        void worker();
        void createThreads(int nThreads);
        
    protected:
        std::vector<std::thread> threads;
        
        std::queue<Job> jobs;
        std::condition_variable signal;
        std::mutex queueMutex;
        
        bool running;
    };
};

#endif //_THREAD_POOL_H
