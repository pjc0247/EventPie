#include "TCPServer.h"

#include "TCPSocket.h"

#include "../EventPie.h"

#include <sys/types.h>
#include <sys/time.h>
#include <sys/event.h>

#include <netinet/in.h>
#include <arpa/inet.h>

#include <thread>

using namespace std;
using namespace EventPie::Config;

namespace EventPie{
    namespace Io{
      
        TCPServer::TCPServer() :
            port(0),
            acceptCallback(nullptr){
                
        }
        TCPServer::TCPServer(
            int _port,
            function<void(int, TCPSocket *)> _callback) :
            port(_port),
            acceptCallback(_callback){
            
            openAsync( port );
        }
        TCPServer::~TCPServer(){
        }
        
        void TCPServer::onRead(){
            TCPSocket *tcp;
            
            sockaddr addr;
            socklen_t addrLen;
            int client;
            
            client = ::accept( fd, &addr, &addrLen );
            tcp = new TCPSocket( client );
            
            Ev::watch( client, Ev::eRead, tcp );
            
            EP_SAFE_CALL( acceptCallback, eNoError, tcp );
        }
        void TCPServer::onWritten(){
            
        }
        
        bool TCPServer::open(int port){
            sockaddr_in addr;
            
            /* create socket */
            fd = ::socket( PF_INET, SOCK_STREAM, 0 );
            if( fd == 0 ){
                EP_SAFE_DEFER( acceptCallback, eSocketError, nullptr );
                return false;
            }
            
            memset( &addr, 0, sizeof(addr) );
            addr.sin_family=AF_INET;
            addr.sin_addr.s_addr = htonl( INADDR_ANY );
            addr.sin_port = htons( port );
            
            /* setopt - reuse address */
            int opt = 1;
            setsockopt(
                fd,
                SOL_SOCKET, SO_REUSEADDR,
                &opt, sizeof(opt) );
            
            /* bind */
            if( ::bind( fd, (sockaddr*)&addr, sizeof(addr) ) == -1 ){
                EP_SAFE_DEFER( acceptCallback, eBindingError, nullptr );
                return false;
            }
            
            /* listen */
            if( ::listen( fd, backlogSize ) == -1 ){
                EP_SAFE_DEFER( acceptCallback, eListeningError, nullptr );
                return false;
            }
            
            Ev::watch( fd, Ev::eRead, this );
            
            return true;
        }
        void TCPServer::openAsync(int port){
            deferAsync(
                [this, port](){
                    open( port );
                });
        }
    };
};
