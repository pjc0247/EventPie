#ifndef _ERROR_H
#define _ERROR_H

namespace EventPie{

    enum Error{
        /* no error */
        eNoError = 0,
        
        /* std error */
        eAllocError = 1000,
        
        /* network error */
        eSocketError = 2000,
        eHostError,
        
        eConnectionError,
        eBindingError,
        eListeningError,
        
        /* unknown error */
        eUnknownError=9000
    };
};

#endif //_ERROR_H
