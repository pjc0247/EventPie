#ifndef _ERROR_H
#define _ERROR_H

namespace EventPie{

    enum Error{
        /* no error */
        eNoError = 0,
        
        /* std error */
        eAllocError = 1000,
        
        eParseError = 1100,
        
        /* network error */
        eSocketError = 2000,
        eHostError,
        
        eConnectionError,
        eBindingError,
        eListeningError,
        
        /* ev error */
        eEvInitializeError = 3000,
        
        /* unknown error */
        eUnknownError=9000
    };
};

#endif //_ERROR_H
