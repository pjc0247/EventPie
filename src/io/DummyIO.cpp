#include "DummyIO.h"

#include "../EventPie.h"

#include <chrono>
#include <thread>

using namespace std;
using namespace std::chrono;

namespace EventPie{
    namespace DummyIO{
        
        void verySlowIO(
            int n,
            std::function<void(int)> callback){
            
            defer( [n, callback](){
                this_thread::sleep_for(
                    milliseconds(1500) );

                addTask(
                    std::bind( callback, n+1 ) );
            });
        }
        
    };
};
