#include <iostream>
#include <thread>
#include <vector>
#include <hiredis/adapters/libuv.h>
#include <hiredis/async.h>
#include <hiredis/hiredis.h>
#include <uv.h>

redisAsyncContext * c2;

void
connectCallback(
    const redisAsyncContext * conn,
    int status) {
    
    printf("connectCallback status:%d\n", status);
}

void
disconnectCallback(
    const redisAsyncContext * conn,
    int status) {

    printf("disconnectCallback status:%d\n", status);

}
int main(int argc, char * argv[]){
    uv_loop_t* loop = uv_default_loop();

    c2 = redisAsyncConnect("192.168.0.35", 6380);


    redisLibuvAttach(c2, loop);
    redisAsyncSetConnectCallback(c2, connectCallback);
    redisAsyncSetDisconnectCallback(c2, disconnectCallback);

    uv_run(loop, UV_RUN_DEFAULT);

    printf("end\n");
    return 0;
}