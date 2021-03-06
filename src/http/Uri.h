#ifndef _HTTP_URI_H
#define _HTTP_URI_H

#include <string>

namespace EventPie{
    namespace Http{
        
        /* http://www.naver.com:80/main/index.html */
        /* protocol : http|https */
        /* domain : www.naver.com */
        /* port : 80 */
        /* request uri : main/index.html */
        /* */
        /* regex : ^(http|https):\/\/([\.a-zA-Z0-9_-]*):?([0-9]*)?\/?([\.\/a-z]*)$ */
    
        class Uri{
        public:
            Uri();
            Uri(const std::string &uri);
            virtual ~Uri();
            
            const std::string &getProtocol() const;
            const std::string &getDomain() const;
            const std::string &getRequestUri() const;
            int getPort() const;
            
        protected:
            void parse();
            
        protected:
            std::string uri;
            
            std::string protocol, domain, requestUri;
            int port;
        };
    };
};

#endif //_HTTP_URI_H
