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

namespace EventPie{
    namespace Io{
        
        struct TCPServerData{
            int kq;
            
            TCPServerData(){
                kq = 0;
            }
        };
      
        TCPServer::TCPServer() :
            port(0), sock(0),
            acceptCallback(nullptr){
                
        }
        TCPServer::TCPServer(
            int _port,
            function<void(TCPSocket *)> _callback) :
            port(_port), sock(0),
            acceptCallback(_callback){
            
            data = new TCPServerData();
                
            open();
            runAsync();
        }
        TCPServer::~TCPServer(){
            delete data;
        }
        
        bool TCPServer::open(){
            sockaddr_in addr;
            
            sock = ::socket( PF_INET, SOCK_STREAM, 0 );
            if( sock == 0 )
                return false;
            
            memset( &addr, 0, sizeof(addr) );
            addr.sin_family=AF_INET;
            addr.sin_addr.s_addr = htonl( INADDR_ANY );
            addr.sin_port = htons( port );
            
            /* bind */
            if( ::bind( sock, (sockaddr*)&addr, sizeof(addr) ) == -1 )
                return false;
            
            /* listen */
            if( ::listen( sock, 5 ) == -1 )
                return false;
            
            return true;
        }
        void TCPServer::run(){
            struct kevent event;
            
            data->kq = kqueue();
            
            EV_SET(
                &event, sock,
                EVFILT_READ, EV_ADD | EV_ENABLE,
                0,0,0 );
            kevent( data->kq, &event, 1, 0,0,0 );
            
            while( true ){
                struct kevent events[1024];
                struct timespec timeout={5,0};
                
                int nEvents = kevent( data->kq, 0,0, events, 1024, &timeout );
                
                for(int i=0;i<nEvents;i++){
                    struct kevent &ev = events[i];
                    
                    if( ev.ident == sock ){
                        TCPSocket *client = accept();
                        
                        EP_SAFE_DEFER( acceptCallback, client );
                    }
                    else{
                        TCPSocket *client = (TCPSocket *)ev.udata;
                        
                        client->receive();
                    }
                }
            }
            
        }
        void TCPServer::runAsync(){
            thread(
                bind( &TCPServer::run, this ) ).detach();
        }
        
        TCPSocket *TCPServer::accept(){
            TCPSocket *tcp;
            struct kevent event;
            
            sockaddr addr;
            socklen_t addrLen;
            int client;
            
            client = ::accept( sock, &addr, &addrLen );
            
            tcp = new TCPSocket( client );
            
            EV_SET(
                &event, client,
                EVFILT_READ, EV_ADD,
                0,0, tcp );
            kevent( data->kq, &event, 1, 0,0,0 );
            
            return tcp;
        }
        
    };
};
