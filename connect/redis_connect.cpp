#include <iostream>
#include <thread>
#include <vector>
#include <hiredis/hiredis.h>

redisContext *c;

void 
connect(){
    printf("start connect\n");
    c = redisConnect("127.0.0.1", 6380); // Blocking
    if (c == NULL || c->err) {
        if (c) {
            printf("Error: %s\n", c->errstr);
        } else {
            printf("Can't allocate redis context\n");
        }
        return;
    }
    printf("Redis connect success\n");
}


void 
connectWithTimeout(){
    printf("start connectWithTimeout\n");
    struct timeval tv;
    tv.tv_sec = 10;
    tv.tv_usec = 0;
    c = redisConnectWithTimeout("125.209.222.142", 6380, tv); // Blocking
    if (c == NULL || c->err) {
        if (c) {
            printf("Error: %s\n", c->errstr);
        } else {
            printf("Can't allocate redis context\n");
        }
        return;
    }
    printf("Redis connect success\n");
}

void 
connectWithOptions(){
    printf("start connectWithOptions\n");
    struct timeval tv;
    tv.tv_sec = 10;
    tv.tv_usec = 0;

    redisOptions *option = (redisOptions *)malloc(sizeof(redisOptions));
    REDIS_OPTIONS_SET_TCP(option, "125.209.222.142", 6380);
    option->connect_timeout = &tv;
    
    c = redisConnectWithOptions(option); // Blocking
    if (c == NULL || c->err) {
        if (c) {
            printf("Error: %s\n", c->errstr);
        } else {
            printf("Can't allocate redis context\n");
        }
        return;
    }
    printf("Redis connect success\n");
}

void 
connectWithUnix(){
    c = redisConnectUnix("/tmp/redis.sock"); // Blocking
    if (c == NULL || c->err) {
        if (c) {
            printf("Error: %s\n", c->errstr);
        } else {
            printf("Can't allocate redis context\n");
        }
        return;
    }
    printf("Redis connect success\n");
}
int main(int argc, char * argv[]){

    auto t = std::thread([]{
        // connect();
        // connectWithTimeout();
        // connectWithOptions();
        connectWithUnix();

        redisReply * reply;
        int ret;
        int i = 0;
        while(1){
        }
    });




    t.join();

    printf("end\n");
    return 0;
}