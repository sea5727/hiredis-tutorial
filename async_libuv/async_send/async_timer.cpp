#include <iostream>
#include <thread>
#include <vector>
#include <uv.h>
#include <hiredis/adapters/libuv.h>
#include <hiredis/async.h>
#include <hiredis/hiredis.h>

redisAsyncContext * c2;
redisReply * reply;
uv_loop_t* loop;
bool connected = false;

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
}

void
connectCallback(
    const redisAsyncContext * conn,
    int status) {
    connected = true;
    printf("connection success\n");
}

void
disconnectCallback(
    const redisAsyncContext * conn,
    int status) {

    printf("disconnectCallback status:%d\n", status);

}

void
closeCb(
    uv_handle_t * handle){
    free(handle->data);
}


void
asyncCallback2(
    uv_async_t * handle){
    static int count = 0;
    printf("asyncCallback2 i:%d\n", count);
    count += 1;

    uv_close((uv_handle_t *)handle, closeCb);
}


void
timer_cb(uv_timer_t * handle){
    printf("timer_cb call\n");
}


void * thread_f(void *){
    while(!connected);
    int i = 0;
    int ret;
    for(i = 0 ; i < 1000 ; ++i){ // 10 ~ 10000
        printf("redisAsyncCommand i:%d\n", i);
        redisAsyncCommand(c2, redisAsyncCommandCallback, NULL,  "%s %s%d %s", "SET", "KEY", i,  "VALUE3");
    }

}
int main(int argc, char * argv[]){
    
   loop = uv_default_loop();

    c2 = redisAsyncConnect("192.168.0.35", 6380);


    redisLibuvAttach(c2, loop);
    redisAsyncSetConnectCallback(c2, connectCallback);
    redisAsyncSetDisconnectCallback(c2, disconnectCallback);

    uv_timer_t * timer = (uv_timer_t *)malloc(sizeof(uv_timer_t));
    uv_timer_init(loop, timer);
    uv_timer_start(timer, timer_cb, 1, 1);
    pthread_t pid;
    pthread_create(&pid, NULL, thread_f, NULL);

    uv_run(loop, UV_RUN_DEFAULT);

    return 0;
}