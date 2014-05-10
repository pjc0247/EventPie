#include "Client.h"

#include "../EventPie.h"
#include "../io/TCPSocket.h"

using namespace std;
using namespace EventPie::Io;

namespace EventPie{
    namespace Http{
    
        int offsetOfDelim(const std::string &buffer){
            for(int i=0;i<buffer.size()-3;i++){
                if( buffer[i+0] == '\r' &&
                    buffer[i+1] == '\n' &&
                    buffer[i+2] == '\r' &&
                    buffer[i+3] == '\n' ){
                    
                    return i;
                }
            }
            return -1;
        }
        
        Client::Client(){
            
        }
        Client::Client(const char *_uri, ResponseCallback _callback) :
            uri(_uri),
            responseCallback(_callback){
            
            openAsync();
        }
        Client::Client(Uri &_uri, ResponseCallback _callback) :
            uri(_uri),
            responseCallback(_callback){
                
            openAsync();
        }
        Client::~Client(){
            
        }
        
        void Client::open(){
            TCPSocket *socket = new TCPSocket(
                uri.getDomain().c_str(), uri.getPort(),
                [this](int err, TCPSocket *socket){
                    if( err != eNoError ){
                        EP_SAFE_DEFER( responseCallback, err, "","" );
                    }
                    else{
                        string *buffer = new string();
                        
                        socket->onReceive(
                            [this, socket, buffer](void *data, int len){
                                buffer->append( (char*)data );
                            });
                        socket->onUnbind(
                            [this, socket, buffer](){
                                string header, body;
                                int offset = offsetOfDelim(*buffer);
                                
                                if( offset == - 1 ){
                                    EP_SAFE_DEFER( responseCallback, eParseError, "","" );
                                }
                                else{
                                    header = buffer->substr( 0, offset );
                                    body = buffer->substr( offset+2 );
                                
                                    EP_SAFE_DEFER( responseCallback, eNoError, header, body );
                                }
                                
                                delete buffer;
                            });
                        
                        string header =
                            "GET / HTTP/1.1\r\n"
                            "Accept : */*\r\n"
                            "Connection : Close\r\n\r\n";
                        
                        socket->write(
                            (void*)header.c_str(), header.size());
                    }
                });
        }
        void Client::openAsync(){
            deferAsync(
                [this](){
                    open();
                });
        }
    };
};
