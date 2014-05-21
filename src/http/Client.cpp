#include "Client.h"

#include "../EventPie.h"
#include "../io/TCPSocket.h"

#include "Protocol.h"
#include "Header.h"

using namespace std;
using namespace EventPie::Io;

namespace EventPie{
    namespace Http{
        
        string generateRequestLine(
            Method method, const string &requestUri, const string &version){
            
            string requestLine;
            
            /* method */
            switch( method ){
                case eGet:
                    requestLine = "GET ";
                    break;
                case ePost:
                    requestLine = "POST ";
                    break;
            }
            
            /* request uri */
            requestLine += requestUri;
            requestLine += " ";
            
            /* http version */
            requestLine += version;
            
            /* crlf */
            requestLine += "\r\n";
            
            return requestLine;
        }
        
        Client::Client(){
            
        }
        Client::Client(const string &_uri, ResponseCallback _callback) :
            uri(_uri),
            responseCallback(_callback){
            
            //requestAsync();
        }
        Client::Client(Uri _uri, ResponseCallback _callback) :
            uri(_uri),
            responseCallback(_callback){
                
            requestAsync(
                eGet, uri );
        }
        Client::~Client(){
            
        }
        
        void Client::open(const std::string &host, int port){
            
                
        }
        
        void Client::request(
            Method method, Uri uri,
            void *data, int len){
            
            Header header;
            header.setField(
                "Host", uri.getDomain() );
            header.setField(
                "Connection", "Closed" );
            header.setField(
                "Accept", "*/*" );

            request(
                method, uri,
                header,
                data, len );
        }
        void Client::request(
            Method method, Uri uri,
            Header header,
            void *data, int len){
            
            TCPSocket *socket = new TCPSocket(
                uri.getDomain().c_str(), uri.getPort(),
                [=](int err, TCPSocket *socket){
                    if( err != eNoError ){
                        EP_SAFE_DEFER( responseCallback, err, Header(),"" );
                    }
                    else{
                        string *buffer = new string();
                        
                        socket->onReceive(
                            [=](void *data, int len){
                                buffer->append( (char*)data );
                            });
                        socket->onUnbind(
                            [=](){
                                Header header;
                                string body;
                                int offset = header.load( *buffer );
                                
                                /*
                                 todo : parse chunked content-length
                                 */
                                
                                if( offset == - 1 ){
                                    EP_SAFE_DEFER( responseCallback, eParseError, Header(),"" );
                                }
                                else{
                                    body = buffer->substr( offset );
                                    
                                    header.load( *buffer );
                                
                                    EP_SAFE_DEFER( responseCallback, eNoError, header, body );
                                }
                                
                                delete buffer;
                            });
                        
                        string requestLine =
                            generateRequestLine( method, uri.getRequestUri(), "HTTP/1.1" );
                        string headerString =
                            header.dump();
                        
                        socket->write(
                            (void*)requestLine.c_str(), requestLine.size());
                        socket->write(
                            (void*)headerString.c_str(), headerString.size());
                    }
                });
        }
        void Client::requestAsync(
            Method method, Uri uri,
            void *data, int len){
            
            deferAsync(
                [this, method,uri,data,len](){
                    Header header;
                    
                    request( method, uri, data, len );
                });
        }
        void Client::requestAsync(
            Method method, Uri uri,
            Header header,
            void *data, int len){
            
            deferAsync(
                [this](){
                    
                   // request( header );
                });
        }
    };
};
