#include "ThreadPool.h"

using namespace std;

namespace EventPie{

    ThreadPool::ThreadPool(){
    }
    ThreadPool::ThreadPool(int nThreads){
        createThreads( nThreads );
    }
    ThreadPool::~ThreadPool(){
        kill();
    }
    
    void ThreadPool::createThreads(int nThreads){
        auto boundMethod =
            bind( &ThreadPool::worker, this );
        
        running = true;
        
        for(int i=0;i<nThreads;i++){
            threads.push_back(
                thread( boundMethod ) );
        }
    }
    void ThreadPool::kill(){
        running = false;
        
        signal.notify_all();
    }
    
    void ThreadPool::enqueue(Job job){
        unique_lock<mutex> lock( queueMutex );
            jobs.push( job );
        lock.unlock();
        
        signal.notify_one();
    }
    
    void ThreadPool::worker(){
        
        while( running ){
            Job job;
            
            unique_lock<mutex> lock( queueMutex );
                if( jobs.empty() ){
                    signal.wait( lock );
                
                    if( jobs.empty() )
                        continue;
                }
            
                job = jobs.front();
                jobs.pop();
            lock.unlock();
            
            /* process job */
            job();
        }
    }
};
