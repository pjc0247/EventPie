#ifndef _IO_INTERFACE_H
#define _IO_INTERFACE_H

namespace EventPie{
    namespace Io{
    
        class Interface{
        public:
            Interface();
            Interface(int fd);
            
            virtual void onRead() = 0;
            virtual void onWritten() = 0;
            
            int getFileDescriptor();
            
        protected:
            int fd;
        };;
    };
};

#endif //_IO_INTERFACE_H
