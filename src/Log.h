#ifndef _LOG_H
#define _LOG_H

namespace EventPie{
    namespace Log{
        enum Tag{
            eTagStart = 0,
            
            eTagEnd = 1024
        };
        
        void error(int tag, const char *fmt, ...);
        void warn(int tag, const char *fmt, ...);
        void info(int tag, const char *fmt, ...);
        
        void enable(int tag);
        void disable(int tag);
    };
};

#endif //_LOG_H
