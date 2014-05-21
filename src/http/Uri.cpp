#include "Uri.h"

using namespace std;

namespace EventPie{
    namespace Http{
        /* http://www.naver.com:80/main/index.html */
        /* protocol : http|https */
        /* domain : www.naver.com */
        /* port : 80 */
        /* request uri : main/index.html */
        /* */
        /* regex : ^(http|https):\/\/([\.a-zA-Z0-9_-]*):?([0-9]*)?\/?([\.\/a-z]*)$ */
    
        enum UriParsePhase{
            eProtocol = 0,
            eDomain,
            ePort,
            eRequestUri
        };
        
        Uri::Uri(){
            
        }
        Uri::Uri(const string &_uri) :
            uri(_uri), port(80){
            
            parse();
        }
        Uri::~Uri(){
            
        }
        
        const string &Uri::getProtocol() const{
            return protocol;
        }
        const string &Uri::getDomain() const{
            return domain;
        }
        const string &Uri::getRequestUri() const{
            return requestUri;
        }
        int Uri::getPort() const{
            return port;
        }
        
        void Uri::parse(){
            int phase = eProtocol;
            int offset = 0;
            
            for(int i=0;i<uri.size();i++){
                switch( phase ){
                    case eProtocol:
                        if( uri[i] == ':' ){
                            protocol =
                                uri.substr( 0, i );
                            
                            i += 3;
                            phase = eDomain;
                            offset = i;
                        }
                        break;
                    case eDomain:
                    case ePort:
                        if( uri[i] == ':' ){
                            domain =
                                uri.substr(     offset, i-offset );
                            phase = ePort;
                            offset = i;
                        }
                        else if( uri[i] == '/' ){
                            if( phase == ePort ){
                                sscanf(
                                    uri.substr( offset, i-offset ).c_str(),
                                    ":%d", &port );
                            }
                            else{
                                domain =
                                    uri.substr( offset, i-offset );
                            }
                            phase = eRequestUri;
                            offset = i;
                            break;
                        }
                        break;
                }
            }
            requestUri =
                uri.substr( offset, uri.size() - offset );
        }
    };
};
