#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

#include <hiredis.h>
#include <async.h>
#include <adapters/libuv.h>
#include <vector>
#include <thread>

void connectCallback(const redisAsyncContext *c, int status) ;
void disconnectCallback(const redisAsyncContext *c, int status);
void l_timer_callback(uv_timer_t * handle);
redisAsyncContext *c2;
uv_loop_t* loop;

static void close_cb(uv_handle_t *handle) {
    printf("close_cb(%p)\n", handle);
    free(handle);
}

static void l_close_cb(uv_handle_t *handle) { 
    printf("close_cb(%p)\n", handle);

    uv_timer_init(loop, (uv_timer_t *)handle);
    uv_timer_start((uv_timer_t *)handle, l_timer_callback, 50, 0);

}

void getCallback(redisAsyncContext *c, void *r, void *privdata) {
    redisReply *reply = (redisReply *)r;
    if (reply == NULL) return;
    printf("argv[%s]: %s\n", (char*)privdata, reply->str);

    // /* Disconnect after receiving the reply to GET */
    // redisAsyncDisconnect(c);
}
void timer_callback(uv_timer_t * handle){
    
    uv_timer_stop(handle);
    uv_close((uv_handle_t *)handle, close_cb);

    uv_timer_t * timer2 = (uv_timer_t *)malloc(sizeof(uv_timer_t));
    uv_timer_init(loop, timer2);
    uv_timer_start(timer2, timer_callback, 50, 0);
}

void l_timer_callback(uv_timer_t * handle){

    printf("handle:%p\n", handle);
    
    uv_timer_stop(handle);
    uv_close((uv_handle_t *)handle, l_close_cb); // close 도 비동기적으로 수행, 이후 free나 재할당 해야함
    // uv_timer_start(handle, l_timer_callback, 50, 0);
}


void connectCallback(const redisAsyncContext *c, int status) {
    printf("[connectCallback] start status:%d\n", status);
    if (status != REDIS_OK) {
        printf("[connectCallback] Error: %d, %s, %d, %s\n", c->err, c->errstr, errno, strerror(errno));

        redisAsyncDisconnect(c2);

        uv_timer_t * timer = (uv_timer_t *)malloc(sizeof(uv_timer_t));
        uv_timer_init(loop, timer);
        uv_timer_start(timer, timer_callback, 50, 0);

        return;
    }
    printf("Connected...\n");
}

void disconnectCallback(const redisAsyncContext *c, int status) {
    printf("[disconnectCallback] start status:%d\n", status);
    if (status != REDIS_OK) {
        printf("[disconnectCallback]Error: %s\n", c->errstr);
        return;
    }
    printf("Disconnected...\n");
}

int main (int argc, char **argv) {

    loop = uv_default_loop();

    std::vector<std::thread> workers(2);


    c2 = redisAsyncConnect("192.168.0.35", 6380);
    if (c2->err) {
        /* Let *c leak for now... */
        printf("Error: %s\n", c2->errstr);
        return 1;
    }


    // uv_timer_t * timer = (uv_timer_t *)malloc(sizeof(uv_timer_t));
    // uv_timer_init(loop, timer);
    // uv_timer_start(timer, timer_callback, 50, 0);

    uv_timer_t l_timer;
    uv_timer_init(loop, &l_timer);
    uv_timer_start(&l_timer, l_timer_callback, 50, 0);

    printf("l_timer:%p\n", &l_timer);

    uv_run(loop, UV_RUN_DEFAULT);
    return 0;
}