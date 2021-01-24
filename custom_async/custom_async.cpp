#include <iostream>
#include <stdio.h>
#include <hiredis/async.h>
#include "custom_async.hpp"

redisAsyncContext * ac;
redisReply * reply;

int i = 0;


void 
redisAsyncCommandCallback(
    struct redisAsyncContext* conn, 
    void* r, 
    void* args){

    reply = (redisReply *)r;

    if(reply == nullptr) {
        printf("redisAsyncCommandCallback reply is null\n");
        return;
    }
    printf("redisAsyncCommandCallback..\n");
    // if(i == 100000) exit(0);
    // redisAsyncCommand(c2, redisAsyncCommandCallback, NULL,  "%s %s%d %s", "SET", "KEY", i,  "VALUE3");
    // i += 1;
    
}


void connectCallback(
    const redisAsyncContext* c2, 
    int status){

    printf("connectCallback start\n");

    redisAsyncCommand(ac, redisAsyncCommandCallback, NULL,  "%s %s %s", "SET", "KEY123",  "VALUE3");

}

void disconnectCallback(
    const redisAsyncContext*, 
    int status){

    printf("connectCallback end\n");
}


int main(int argc, char *argv[]){

    std::cout << "Hello World" << std::endl;

    auto epoll = EventCLoop::Epoll{};

    ac = redisAsyncConnect("192.168.0.35", 6380);
    

    redisAttach(ac, &epoll);
    redisAsyncSetConnectCallback(ac, connectCallback);
    redisAsyncSetDisconnectCallback(ac, disconnectCallback);

    while(1){
        epoll.Run();
    }
    
    return 0;

}