#ifndef _TCP_SERVER_H
#define _TCP_SERVER_H

#include <functional>

namespace EventPie{
    namespace Io{
        
        class TCPSocket;
        struct TCPServerData;
        
        class TCPServer{
        public:
            TCPServer();
            TCPServer(
                int port,
                std::function<void(TCPSocket *)> callback);
            virtual ~TCPServer();
            
        protected:
            bool open();
            void run();
            void runAsync();
            
            TCPSocket *accept();
            
        protected:
            int port;
            std::function<void(TCPSocket *)> acceptCallback;
            
            int sock;
            
            TCPServerData *data;
        };
    };
};

#endif //_TCP_SERVER_H
