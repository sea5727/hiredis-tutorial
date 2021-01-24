#include <iostream>
#include <thread>
#include <vector>
#include <hiredis/adapters/libuv.h>
#include <hiredis/async.h>
#include <hiredis/hiredis.h>
#include <uv.h>

redisAsyncContext * c2;
uv_loop_t* loop;

void
connectCallback(
    const redisAsyncContext * conn,
    int status);
void
disconnectCallback(
    const redisAsyncContext * conn,
    int status);

void
connectCallback(
    const redisAsyncContext * conn,
    int status) {
    
    if(status != REDIS_OK){ // connnect
        printf("connectCallback conn:%p, err:%d, errstr:%s, status:%d\n", conn, conn->err, conn->errstr, status);
        
        std::this_thread::sleep_for(std::chrono::milliseconds(30));
        c2 = redisAsyncConnect("192.168.0.35", 6380);

        redisLibuvAttach(c2, loop);
        redisAsyncSetConnectCallback(c2, connectCallback);
        redisAsyncSetDisconnectCallback(c2, disconnectCallback);
        return;
    }
    printf("connection success\n");
}

void
disconnectCallback(
    const redisAsyncContext * conn,
    int status) {

    printf("disconnectCallback status:%d\n", status);

}
int main(int argc, char * argv[]){
    loop = uv_default_loop();

    c2 = redisAsyncConnect("192.168.0.35", 6379);


    redisLibuvAttach(c2, loop);
    redisAsyncSetConnectCallback(c2, connectCallback);
    redisAsyncSetDisconnectCallback(c2, disconnectCallback);

    uv_run(loop, UV_RUN_DEFAULT);

    printf("end\n");
    return 0;
}