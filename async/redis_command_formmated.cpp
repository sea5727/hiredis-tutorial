#include <iostream>
#include <thread>
#include <vector>
#include <hiredis/adapters/libuv.h>
#include <hiredis/async.h>
#include <hiredis/hiredis.h>
#include <uv.h>

redisAsyncContext * c2;
redisReply * reply;
uv_loop_t* loop;

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
    
    printf("reply type:%d, integer:%d, len:%d, str:%s\n", 
        reply->type,
        reply->integer,
        reply->len,
        reply->str);
    return;

    
}

void
connectCallback(
    const redisAsyncContext * conn,
    int status) {

    printf("connection success\n");

    // redisAsyncCommand(c2, redisAsyncCommandCallback, NULL,  "%s %s%d %s", "SET", "KEY", i,  "VALUE3");
    char cmd[1024] = "SET";
    char key[1024] = "KEY0";
    char value[1024] = "VALUE3";

    const char * arr[] = {cmd, key, value};
    size_t sizes[] = { 3, 4, 6};
    redisAsyncCommandArgv(c2, redisAsyncCommandCallback, NULL, 3, arr, sizes);
}

void
disconnectCallback(
    const redisAsyncContext * conn,
    int status) {

    printf("disconnectCallback status:%d\n", status);

}
int main(int argc, char * argv[]){
    loop = uv_default_loop();

    c2 = redisAsyncConnect("192.168.0.35", 6380);


    redisLibuvAttach(c2, loop);
    redisAsyncSetConnectCallback(c2, connectCallback);
    redisAsyncSetDisconnectCallback(c2, disconnectCallback);

    uv_run(loop, UV_RUN_DEFAULT);

    printf("end\n");
    return 0;
}