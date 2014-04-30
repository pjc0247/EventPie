#ifndef _TCP_SOCKET_H
#define _TCP_SOCKET_H

#include <functional>

namespace EventPie{
    namespace Io{
        
        class TCPServer;
        
        class TCPSocket{
            friend TCPServer;
            
        public:
            static const int InbufSize = 1024;
            
        public:
            TCPSocket();
            TCPSocket(int sock);
            virtual ~TCPSocket();
            
            void onReceive(std::function<void(void*,int)> callback);
            void onUnbind(std::function<void()> callback);
            
            void write(void *data,int len);
            void close();
            
            void setNodelay(bool nodelay);
            
        protected:
            void receive();
            void unbind();
            
        protected:
            int sock;
            char inbuf[InbufSize];
            
            std::function<void(void*,int)> receiveCallback;
            std::function<void()> unbindCallback;
        };
    };
};

#endif //_TCP_SOCKET_H
