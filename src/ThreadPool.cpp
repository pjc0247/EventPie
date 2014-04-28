#include "ThreadPool.h"

using namespace std;

namespace EventPie{

    ThreadPool::ThreadPool(){
        createThreads( THREAD_POOL_DEFAULT_SIZE );
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
		
		for( auto &thread : threads )
			thread.detach();
		threads.clear();

		int spinCount = 100000;
		while( nThreads.load() > 0 ){
			if( spinCount )
				spinCount --;
			else{
				std::this_thread::sleep_for(
					std::chrono::milliseconds(1) );
			}
		}
    }
    
    void ThreadPool::enqueue(Task task){
        unique_lock<mutex> lock( queueMutex );
            tasks.push( task );
        lock.unlock();
        
        signal.notify_one();
    }
    
    void ThreadPool::worker(){
		nThreads.fetch_add(1);

        while( running ){
            Task task;
			
            unique_lock<mutex> lock( queueMutex );
                if( tasks.empty() ){
                    signal.wait( lock );
                
                    if( tasks.empty() )
                        continue;
                }
            
                task = tasks.front();
                tasks.pop();
            lock.unlock();
            
            /* process task */
            task();
        }
		
		nThreads.fetch_sub(1);
    }
};
