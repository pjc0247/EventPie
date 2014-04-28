#ifndef _SOCKET_H
#define _SOCKET_H

#include <functional>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

namespace EventPie{
    /*
     * namespace Socket
     * 
     */
    namespace Socket{
      
        void accept(
            int socket,
            std::function<void(int, sockaddr, socklen_t)> callback);
        
        void send(
            int socket, void *data, size_t len, int flag,
            std::function<void(void *data,size_t len)> callback);
    };
};

#endif //_SOCKET_H
