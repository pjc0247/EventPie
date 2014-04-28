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

Features
----

__Timer__

타이머는 지정된 시간 주기마다 반복적으로 콜백을 불러줍니다.

```C++
using namespace EventPie;

int main(){
  int life = 3;
  Timer *timer = new Timer( [counter](){
    printf("0.15초마다 실행되는 타이머\n");
    
    life --;
    if( life == 0 )
      timer->stop();
  }, 150);
  
  run();
}
```

__Task__

태스크는 EventPie에서 처리하는 작업의 단위입니다.



addTask함수를 사용하여 태스크을 뒤로 미룰 수 있습니다.

( addTask함수는 node-js의 nextTick과 같은 역할을 합니다. )
```C++
using namespace EventPie;

int main(){
  addTask( [](){
    printf("
  });
  
  run();
}
```

defer함수를 사용하여 태스크를 비동기적으로 수행할 수 있습니다.

태스크는 EventPie내부의 ThreadPool에서 수행됩니다.

( 이 함수를 통해서 blocking 로직을 non-blocking로직으로 변환할 수 있습니다. )
```C++
using namespace EventPie;

int main(){

  /* very slow task */
  defer( [](){
    int result = 0;
    
    sleep(1000);
    result = 1234;
    
    addTask( [result](){
      printf("task completed %d\n", result);
    });
  });
  
  run();
}
```

Comment
----

IO/Socket의 구현체는 단순 시연을 위해 blocking 함수를 강제적으로 non-blocking으로 변환한 것들입니다.
