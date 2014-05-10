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
using namespace EventPie::Config;

namespace EventPie{
    namespace Io{
        
        TCPSocket::TCPSocket() :
            receiveCallback(nullptr), unbindCallback(nullptr){
            
        }
        TCPSocket::TCPSocket(int _sock) :
            Interface(_sock),
            receiveCallback(nullptr), unbindCallback(nullptr){
            
        }
        TCPSocket::TCPSocket(
            const char *host, int port,
            std::function<void(int, TCPSocket*)> callback) :
            connectedCallback(callback),
            receiveCallback(nullptr), unbindCallback(nullptr){
            
            openAsync( host, port );
        }
        TCPSocket::~TCPSocket(){
            
        }
        
        void TCPSocket::onRead(){
            int len = read( fd, inbuf,InbufSize );
            
            if( len == 0 ){
                Ev::unwatch( fd, Ev::eRead );
                
                EP_SAFE_CALL_0( unbindCallback );
            }
            else{
                inbuf[ len ] = 0;
                
                EP_SAFE_CALL( receiveCallback, inbuf,len );
            }
        }
        void TCPSocket::onWritten(){
        }
        
        void TCPSocket::onReceive(function<void(void*,int)> callback){
            receiveCallback = callback;
        }
        void TCPSocket::onUnbind(function<void()> callback){
            unbindCallback = callback;
        }
        
        void TCPSocket::write(void *data,int len){
            ::write( fd, data, len );
        }
        void TCPSocket::close(){
            Ev::unwatch( fd, Ev::eRead );
            
            ::close( fd );
        }
        
        void TCPSocket::setNodelay(bool nodelay){
            int opt = nodelay;
            setsockopt(
                fd,
                IPPROTO_TCP, TCP_NODELAY,
                &opt, sizeof(opt));
        }
        
        bool TCPSocket::open(const char *host, int port){
            hostent *hostInfo;
            sockaddr_in addr;
            
            /* create socket */
            fd = ::socket( PF_INET, SOCK_STREAM, 0 );
            if( fd == 0 ){
                EP_SAFE_DEFER( connectedCallback, eSocketError, this );
                return false;
            }
            
            /* query host address */
            hostInfo = gethostbyname( host );
            if( hostInfo == nullptr ){
                EP_SAFE_DEFER( connectedCallback, eHostError, this );
                return false;
            }
            
            memset( &addr, 0, sizeof(addr) );
            addr.sin_family=AF_INET;
            addr.sin_addr.s_addr = *(in_addr_t*)hostInfo->h_addr_list[0];
            addr.sin_port = htons( port );
            
            /* connect to host */
            if( connect( fd, (sockaddr*)&addr, sizeof(addr) ) == -1 ){
                EP_SAFE_DEFER( connectedCallback, eConnectionError, this );
                return false;
            }
            
            Ev::watch( fd, Ev::eRead, this );
            
            EP_SAFE_DEFER( connectedCallback, eNoError, this );
            return true;
        }
        void TCPSocket::openAsync(const char *host, int port){
            deferAsync(
                [this, host, port](){
                    open( host, port );
                });
        }
    };
};
