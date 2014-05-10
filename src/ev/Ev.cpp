#include "Ev.h"

#include "../Config.h"

#include <sys/types.h>
#include <sys/time.h>
#include <sys/event.h>

#include <unistd.h>

#include <stdio.h>
#include <errno.h>
#include <string.h>


using namespace EventPie::Config;

#define TO_EVFILT(src,dst) do{\
        dst = 0;\
        if( src & eRead )\
            dst |= EVFILT_READ;\
        if( src & eWrite )\
            dst |= EVFILT_WRITE;\
    }while(0);

namespace EventPie{
    namespace Ev{
        
        static int kq;
        
        bool initialize(){
            kq = kqueue();
            
            if( kq == -1 )
                return false;
            else
                return true;
        }
        void quit(){
            close( kq );
        }
        
        void watch(int fd,int filter, void *userData){
            struct kevent event;
            uint16_t kfilter;
            
            /* convert Ev::Filter to EVFILT */
            TO_EVFILT( filter, kfilter );
            
            /* modify change to kqueue */
            EV_SET(
                &event, fd,
                kfilter, EV_ADD | EV_ENABLE,
                0,0, userData );
            kevent( kq, &event, 1, 0,0,0 );
        }
        void unwatch(int fd,int filter){
            struct kevent event;
            uint16_t kfilter;
            
            /* convert Ev::Filter to EVFILT */
            TO_EVFILT( filter, kfilter );
            
            /* modify change to kqueue */
            EV_SET(
                &event, fd,
                kfilter, EV_DELETE,
                0,0,0 );
            kevent( kq, &event, 1, 0,0,0 );
        }
        
        int poll(Event **_events){
            static Event events[ evMaxEvents ];
            static struct kevent kevents[ evMaxEvents ];
            static struct timespec timeout={0,0};
            int nChanges;
            
            nChanges = kevent( kq, 0,0, kevents, 1024, &timeout );
            *_events = events;

           // printf(" changes %d\n", nChanges );
            
            /* copy kevents to events */
            for(int i=0;i<nChanges;i++){
                /* filter */
                switch( kevents[i].filter ){
                    case EVFILT_READ:
                        events[i].filter = eRead;
                        break;
                    case EVFILT_WRITE:
                        events[i].filter = eWrite;
                        break;
                }
                
                /* file descriptor */
                events[i].fd = kevents[i].ident;
                
                /* user data */
                events[i].userData = kevents[i].udata;
            }
            
            return nChanges;
        }
    };
};
