#ifndef _UTIL_H
#define _UTIL_H

#define EP_SAFE_CALL_0(f) do{\
    if( f != nullptr )\
        f();\
    }while(0);
#define EP_SAFE_CALL(f,...) do{\
    if( f != nullptr )\
        f( __VA_ARGS__ );\
    }while(0);

#define EP_SAFE_DEFER_0(f) do{\
    if( f != nullptr ){\
        EventPie::defer( f );\
    }}while(0);
#define EP_SAFE_DEFER(f,...) do{\
    if( f != nullptr ){\
        EventPie::defer(\
            std::bind(f,__VA_ARGS__) );\
    }}while(0);

#endif //_UTIL_H
