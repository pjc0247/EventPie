EventPie
========

pie event pie event pie


EventPie는 C++로 작성된 node-js스타일의 이벤트 드리븐 구현체입니다.

Sample EchoServer
----
```C++

using namespace EventPie;
using namespace EventPie::Io;

int main(){
    TCPServer *server;
    
    server = new TCPServer( 9916,
        [&server](int err, TCPSocket *client){
            
            if( err == eNoError ){
                printf("new connection\n");
                
                client->onReceive(
                    [client](void *data, int len){
                        printf("data received : %s\n", data);
                        
                        /* echo back */
                        client->write( data, len );
                    } );
                client->onUnbind(
                    [client](){
                        printf("client disconnected\n");
                    } );
            }
            else{
                printf("cannot create tcp server - %d\n", err);
            }
        } );
        
    run();  
}
```

Features
----

__Timer__

타이머는 지정된 시간 주기마다 반복적으로 콜백을 불러줍니다.

```C++
using namespace EventPie;

int main(){
  int life = 3;
  Timer *timer = new Timer( [life](){
    printf("0.15초마다 실행되는 타이머\n");
    
    life --;
    if( life == 0 )
      delete timer;
  }, 150);
  
  run();
}
```

__Task__

태스크는 EventPie에서 처리하는 작업의 단위입니다.



defer함수를 사용하여 태스크을 뒤로 미룰 수 있습니다.

( defer함수는 node-js의 nextTick과 같은 역할을 합니다. )
```C++
using namespace EventPie;

int main(){
  defer( [](){
    printf("task\n");
  });
  
  run();
}
```

deferAsync함수를 사용하여 태스크를 비동기적으로 수행할 수 있습니다.

태스크는 EventPie내부의 ThreadPool에서 수행됩니다.

( 이 함수를 통해서 blocking 로직을 non-blocking로직으로 변환할 수 있습니다. )
```C++
using namespace EventPie;

int main(){

  /* very slow task */
  deferAsync( [](){
    int result = 0;
    
    sleep(1000);
    result = 1234;
    
    defer( [result](){
      printf("task completed %d\n", result);
    });
  });
  
  run();
}
```
