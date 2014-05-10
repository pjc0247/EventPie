#include "Interface.h"

namespace EventPie{
    namespace Io{
        
        Interface::Interface() :
            fd(0){
        }
        Interface::Interface(int _fd) :
            fd(_fd){
        }
        int Interface::getFileDescriptor(){
            return fd;
        }
    };
};
