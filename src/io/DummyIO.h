#ifndef _DUMMY_IO_H
#define _DUMMY_IO_H

#include <functional>

namespace EventPie{
    /*
     * namespace DummyIO
     *
     */
    namespace DummyIO{
    
        void verySlowIO(
            int n,
            std::function<void(int)> callback);
        
    };
};

#endif //_DUMMY_IO_H
