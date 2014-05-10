#ifndef _EV_H
#define _EV_H

namespace EventPie{
    namespace Ev{
        
        enum Filter{
            eRead = 1,
            eWrite = 2
        };
        struct Event{
            int fd;
            Filter filter;
            void *userData;
        };
        
        bool initialize();
        void quit();
        
        void watch(int fd,int filter, void *userData);
        void unwatch(int fd,int filter);
        
        int poll(Event **events);
    };
};

#endif //_EV_H
