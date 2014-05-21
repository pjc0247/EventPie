#include "EventPie.h"
#include "http/Client.h"

using namespace EventPie;

int main(int argc,char **argv){
    Http::Uri uri("http://www.naver.com:80/index.html");
    
    new Http::Client(
        uri,
        [](int err, Http::Header header,string body){
            if( err == eNoError ){
                printf("%s", body.c_str());
            }
        });
    
    run();

    return 0;
}
