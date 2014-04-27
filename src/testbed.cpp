#include <iostream>

#include "EventPie.h"
#include "Timer.h"

using namespace EventPie;

int main(int argc, const char * argv[]){

    int a = 1;
    Timer *t = new Timer([&a, &t](){
        printf("on timer %d\n", a++);
        
        if( a > 3 ) t->stop();
    }, 1000);
    
    EventPie::run();
    
    return 0;
}

