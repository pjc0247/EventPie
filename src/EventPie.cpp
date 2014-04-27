#include "EventPie.h"

#include "Event.h"
#include "Timer.h"
#include "Task.h"

#include <queue>
#include <vector>
#include <chrono>

using namespace std;
using namespace std::chrono;

namespace EventPie{
    
    static vector<Timer*> timers;

    static bool running;
    
    int run(){
        running = true;
        
        auto timePoint = system_clock::now();
        
        while( running ){
            /* process event queue */
            
            if( !running ) break;
            
            /* process timers */
            auto loopTime =
                duration_cast<milliseconds>(system_clock::now() - timePoint).count();
            
            if( loopTime ){
                timePoint = system_clock::now();
            
                for( auto &timer : timers ){
                    timer->step( (int)loopTime );
                }
            }
        }
        return 0;
    }
    void stop(){
        running = false;
    }
    
    void addTimer(Timer *timer){
        timers.push_back( timer );
    }
    void removeTimer(Timer *timer){
        for(int i=0;i<timers.size();i++){
            if( timers[i] == timer ){
                std::swap( timers[i], timers.back() );
                timers.pop_back();
            }
        }
    }
}
