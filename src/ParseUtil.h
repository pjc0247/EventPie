#ifndef _PARSE_UTIL_H
#define _PARSE_UTIL_H

#define EP_STRCMP2(str, a1,a2) \
    (*(unsigned short*)(str) == ((a2<<8) | a1))
#define EP_STRCMP4(str, a1,a2,a3,a4) \
    (*(unsigned int*)(str) == ((a4<<24) | (a3<<16) | (a2<<8) | a1))

#endif //_PARSE_UTIL_H
