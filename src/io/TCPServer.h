#ifndef _IO_TCP_SERVER_H
#define _IO_TCP_SERVER_H

#include "Interface.h"

#include <functional>

namespace EventPie{
    namespace Io{
        
        class TCPSocket;
        
        class TCPServer : Interface{
        public:
            TCPServer();
            TCPServer(
                int port,
                std::function<void(int, TCPSocket *)> callback);
            virtual ~TCPServer();
            
        protected:
            virtual void onRead();
            virtual void onWritten();
            
            bool open(int port);
            void openAsync(int port);
            
        protected:
            int port;
            std::function<void(int, TCPSocket *)> acceptCallback;
        };
    };
};

#endif //_IO_TCP_SERVER_H
