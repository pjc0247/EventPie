#include "Socket.h"

#include "../Reaction.h"
#include "../EventPie.h"

#include <thread>

using namespace std;

namespace EventPie{
    namespace Socket{
      
        void accept(
            int socket,
            function<void(int, sockaddr, socklen_t)> callback){
            
            defer( [socket, callback](){
                sockaddr addr;
                socklen_t addrLen;
                int clientSocket;
                
                clientSocket = ::accept( socket, &addr, &addrLen );
                
                addTask(
                    std::bind( callback, clientSocket, addr, addrLen ) );
            });
        }
        
        void send(
            int socket, void *data, size_t len, int flag,
            function<void(void *data,size_t len)> callback){
            
            defer( [socket, data, len, flag, callback](){
                ::send( socket, data, len, flag );
                
                addTask(
                    std::bind( callback, data, len ) );

            });
        }
    };
};
