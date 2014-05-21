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
        
        /* request with uri */
        Client::Client(
            Uri _uri,
            ResponseCallback _callback) :
            uri(_uri),
            responseCallback(_callback){
                
            requestAsync(
                eGet, uri );
        }
        /* request with method and uri */
        Client::Client(
            Method method, Uri _uri,
            ResponseCallback _callback) :
            uri(_uri),
            responseCallback(_callback){
            
            requestAsync(
                method, uri );
        }
        /* request with method, uri and header */
        Client::Client(
            Method method, Uri _uri,
            Header header,
            ResponseCallback _callback) :
            uri(_uri),
            responseCallback(_callback){
            
            requestAsync(
                method, uri, header );
        }
        /* request with method, uri, header and data */
        Client::Client(
            Method method, Uri _uri,
            Header header,
            void *data, int len,
            ResponseCallback _callback) :
            uri(_uri),
            responseCallback(_callback){
            
            requestAsync(
                method, uri, header,
                data, len );
        }
        Client::~Client(){
            
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
            
            /* content-length */
            if( len > 0 ){
                char lenString[16];
                sprintf( lenString, "%d", len );
                
                header.setField(
                    "Content-Length", lenString );
            }
            
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
                                header.getField(
                                    "Transfer-Encoding");
                                
                                if( offset == - 1 ){
                                    EP_SAFE_DEFER( responseCallback, eParseError, Header(),"" );
                                }
                                else{
                                    body = buffer->substr( offset );
                                    
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
                        
                        if( len > 0 ){
                            socket->write( data, len );
                        }
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
                [this, method,uri,header,data,len](){
                    
                    request( method, uri, header, data, len );
                });
        }
    };
};
