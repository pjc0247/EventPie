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
            TCPSocket(
                const char *host, int port,
                std::function<void(int)> callback);
            virtual ~TCPSocket();
            
            void onReceive(std::function<void(void*,int)> callback);
            void onUnbind(std::function<void()> callback);
            
            void write(void *data,int len);
            void close();
            
            void setNodelay(bool nodelay);
            
        protected:
            bool open(const char *host, int port);
            void openAsync(const char *host, int port);
            
            void receive();
            
        protected:
            int sock;
            char inbuf[InbufSize];
            
            std::function<void(void*,int)> receiveCallback;
            std::function<void()> unbindCallback;
            std::function<void(int)> connectedCallback;
        };
    };
};

#endif //_TCP_SOCKET_H
