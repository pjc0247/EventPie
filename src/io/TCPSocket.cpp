#include "TCPSocket.h"

#include "../EventPie.h"

#include <netinet/in.h>
#include <netinet/tcp.h>

#include <arpa/inet.h>

#include <unistd.h>
#include <fcntl.h>

using namespace std;

namespace EventPie{
    namespace Io{
        
        TCPSocket::TCPSocket() :
            sock(0),
            receiveCallback(nullptr), unbindCallback(nullptr){
            
        }
        TCPSocket::TCPSocket(int _sock) :
            sock(_sock),
            receiveCallback(nullptr), unbindCallback(nullptr){
            
        }
        TCPSocket::~TCPSocket(){
            
        }
        
        void TCPSocket::onReceive(function<void(void*,int)> callback){
            receiveCallback = callback;
        }
        void TCPSocket::onUnbind(function<void()> callback){
            unbindCallback = callback;
        }
        
        void TCPSocket::write(void *data,int len){
            ::write( sock, data, len );
        }
        void TCPSocket::close(){
            ::close( sock );
        }
        
        void TCPSocket::setNodelay(bool nodelay){
            int opt = nodelay;
            setsockopt(
                sock,
                IPPROTO_TCP, TCP_NODELAY,
                &opt, sizeof(opt));
        }
        
        void TCPSocket::receive(){
            int len = read( sock, inbuf,InbufSize );
            
            if( len == 0 ){
                EP_SAFE_DEFER_0( unbindCallback );
            }
            else{
                inbuf[ len ] = 0;
                
                EP_SAFE_DEFER( receiveCallback, inbuf,len );
            }
        }
    };
};
