EvnetPie
========

pie event pie event pie


간단한 event-driven 구현체

Sample
----
```C++

using namespace EventPie;

int main(){
  int serverSocket;
  
  /* setup */
  
  Socket::accept( socket,
    [](int clientSocket, sockaddr addr, socklen_t addrLen){
      printf("new connection\n");
      
      Socket::recv( clientSocket,
        [clientSocket](void *data,int len){
          printf("data received\n");
      });
  });
  
  run();  
}
```
