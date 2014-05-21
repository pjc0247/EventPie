#ifndef _HTTP_CLIENT_H
#define _HTTP_CLIENT_H

#include <functional>
#include <string>

#include "../io/TCPSocket.h"

#include "Protocol.h"
#include "Uri.h"

namespace EventPie{
    namespace Http{
        
        class Header;
        typedef std::function<void(int, Header,std::string)> ResponseCallback;
        
        class Client{
        public:
            Client();
            Client(
                const std::string &uri,
                ResponseCallback callback);
            Client(
                Uri uri,
                ResponseCallback callback);
            virtual ~Client();
            
        protected:
            void open(const std::string &host, int port);
            
            void request(
                Method method, Uri uri,
                void *data=nullptr, int len=0);
            void request(
                Method method, Uri uri,
                Header header,
                void *data=nullptr, int len=0);
            
            void requestAsync(
                Method method, Uri uri,
                void *data=nullptr, int len=0);
            void requestAsync(
                Method method, Uri uri,
                Header header,
                void *data=nullptr, int len=0);

        protected:
            Uri uri;
            
            Io::TCPSocket *socket;
            
            ResponseCallback responseCallback;
        };
    };
};

#endif //_HTTP_CLIENT_H
