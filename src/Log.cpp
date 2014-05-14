#include "Log.h"

#include <stdio.h>
#include <stdarg.h>

#include <time.h>
#include <chrono>

#define OUTPUT(tag, prefix, fmt) {\
    if( tagState[tag] == 1 )\
        return;\
    char buffer[1024];\
    auto time = system_clock::to_time_t( system_clock::now() );\
    struct tm *t = localtime(&time);\
    va_list args;\
    va_start( args, fmt );\
        vsprintf( buffer, fmt, args );\
    va_end( args );\
    printf("[%d-%d-%d/%d:%d:%d][" prefix "] %s\n",\
        t->tm_year+1900, t->tm_mon+1, t->tm_mday,\
        t->tm_hour, t->tm_min, t->tm_min,\
        buffer);\
}

using namespace std::chrono;

namespace EventPie{
    namespace Log{
        
        enum State{
            Enabled = 0,
            Disabled = 1
        };
        static bool tagState[eTagEnd] = {0,};
        
        void error(int tag, const char *fmt, ...){
            OUTPUT( tag, "error", fmt );
        }
        void warn(int tag, const char *fmt, ...){
            OUTPUT( tag, "warn", fmt );
        }
        void info(int tag, const char *fmt, ...){
            OUTPUT( tag, "info", fmt );
        }
        
        void enable(int tag){
            tagState[ tag ] = Enabled;
        }
        void disable(int tag){
            tagState[ tag ] = Disabled;
        }
    };
};
