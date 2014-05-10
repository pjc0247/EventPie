#ifndef _HTTP_CLIENT_H
#define _HTTP_CLIENT_H

#include <functional>
#include <string>

#include "Uri.h"

namespace EventPie{
    namespace Http{
        typedef std::function<void(int, std::string,std::string)> ResponseCallback;
        
        class Client{
        public:
            Client();
            Client(const char *uri, ResponseCallback callback);
            Client(Uri &uri, ResponseCallback callback);
            virtual ~Client();
            
        protected:
            void open();
            void openAsync();

        protected:
            Uri uri;
            
            ResponseCallback responseCallback;
        };
    };
};

#endif //_HTTP_CLIENT_H
