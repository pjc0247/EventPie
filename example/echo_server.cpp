#include "EventPie.h"
#include "io/TCPServer.h"

using namespace EventPie;
using namespace EventPie::Io;

const int port = 9916;

int main(int argc,char **argv){
  TCPServer *server;
  
  server = new TCPServer( port,
    [server](TCPClient *client){
      printf("new connection\n");
      
      client->onReceive(
        [client](void *data, int len){
          client->write( data, len );
        });
      client->onUnbind(
        [client](){
          printf("client disconnected\n");
        });
    });
  return 0;
}
