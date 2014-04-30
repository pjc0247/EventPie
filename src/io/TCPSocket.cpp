#include "TCPSocket.h"

#include "../EventPie.h"

#include <netinet/in.h>
#include <netinet/tcp.h>

#include <arpa/inet.h>

#include <netdb.h>

#include <unistd.h>
#include <fcntl.h>

#include <thread>

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
        TCPSocket::TCPSocket(
            const char *host, int port,
            std::function<void(int)> callback) :
            sock(0),
            connectedCallback(callback),
            receiveCallback(nullptr), unbindCallback(nullptr){
            
            openAsync( host, port );
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
        
        bool TCPSocket::open(const char *host, int port){
            hostent *hostInfo;
            sockaddr_in addr;
            
            sock = ::socket( PF_INET, SOCK_STREAM, 0 );
            if( sock == 0 ){
                EP_SAFE_DEFER( connectedCallback, eSocketError );
                return false;
            }
            
            hostInfo = gethostbyname( host );
            if( hostInfo == nullptr ){
                EP_SAFE_DEFER( connectedCallback, eHostError );
                return false;
            }
            
            memset( &addr, 0, sizeof(addr) );
            addr.sin_family=AF_INET;
            addr.sin_addr.s_addr = *(in_addr_t*)hostInfo->h_addr_list[0];
            addr.sin_port = htons( port );
            
            if( connect( sock, (sockaddr*)&addr, sizeof(addr) ) == -1 ){
                EP_SAFE_DEFER( connectedCallback, eConnectionError );
                return false;
            }
            
            EP_SAFE_DEFER( connectedCallback, eNoError );
            return true;
        }
        void TCPSocket::openAsync(const char *host, int port){
            deferAsync(
                [this, host, port](){
                    open( host, port );
                });
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
