#ifndef _IO_TCP_SOCKET_H
#define _IO_TCP_SOCKET_H

#include "Interface.h"

#include <functional>

namespace EventPie{
    namespace Io{
        
        class TCPServer;
        
        class TCPSocket : Interface{
            friend TCPServer;
            
        public:
            static const int InbufSize = 1024;
            
        public:
            TCPSocket();
            TCPSocket(int sock);
            TCPSocket(
                const char *host, int port,
                std::function<void(int, TCPSocket *socket)> callback);
            virtual ~TCPSocket();
            
            virtual void onRead();
            virtual void onWritten();
            
            void onReceive(std::function<void(void*,int)> callback);
            void onUnbind(std::function<void()> callback);
            
            void write(void *data,int len);
            void close();
            
            void setNodelay(bool nodelay);
            
        protected:
            bool open(const char *host, int port);
            void openAsync(const char *host, int port);
            
        protected:
            char inbuf[InbufSize+1];
            
            std::function<void(void*,int)> receiveCallback;
            std::function<void()> unbindCallback;
            std::function<void(int, TCPSocket *socket)> connectedCallback;
        };
    };
};

#endif //_IO_TCP_SOCKET_H
