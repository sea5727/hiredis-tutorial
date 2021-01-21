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
redisAsyncContext *c2;
uv_loop_t* loop;

static void close_cb(uv_handle_t *handle) {
    printf("close_cb(%p)\n", handle);
    free(handle);
}


void getCallback(redisAsyncContext *c, void *r, void *privdata) {
    redisReply *reply = (redisReply *)r;
    if (reply == NULL) return;
    printf("argv[%s]: %s\n", (char*)privdata, reply->str);

    // /* Disconnect after receiving the reply to GET */
    // redisAsyncDisconnect(c);
}
void timer_callback(uv_timer_t * handle){
    

    c2 = redisAsyncConnect("192.168.0.35", 6380);
    if (c2->err) {
        /* Let *c leak for now... */
        printf("Error: %s\n", c2->errstr);
        return;
    }

    redisLibuvAttach(c2,loop);

    // printf("Timer Call c2:%p, %d, %s, %d, %s\n", c2, c2->err, c2->errstr, errno, strerror(errno));
    
    redisAsyncSetConnectCallback(c2,connectCallback);
    redisAsyncSetDisconnectCallback(c2,disconnectCallback);
    uv_timer_stop(handle);
    uv_close((uv_handle_t *)handle, close_cb);
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

    workers.emplace_back([]{

        // uv_async_t * async = (uv_async_t *)malloc(sizeof(uv_async_t));
        // async->data = c2;
        // uv_async_init(loop, async, [](uv_async_t * handle){
        //     redisAsyncContext * c2 = (redisAsyncContext *)handle->data;

        //     const char *targv[3] = {"SET", "TTTEST", "VVVVALUE"};
        //     size_t targvlen[3];
        //     int targc;

        //     targvlen[0] = 3;
        //     targvlen[1] = 6;
        //     targvlen[2] = 8;

        //     targc = 3;
        //     char * cmdbuf;
        //     int cmdlen = redisFormatCommandArgv(&cmdbuf, targc, targv, targvlen);
        //     redisAsyncFormattedCommand(c2, getCallback, NULL, cmdbuf, cmdlen);

        //     uv_close((uv_handle_t *)handle, NULL);

        //     // char * cmd;
        //     // int len = redisFormatCommand(&cmd, "%s", "SET ASYNCKEY hihi");
        //     // redisFormatCommandArgv(&)
        //     // printf("cmd:%s\n", cmd);
        //     // redisAsyncFormattedCommand(c2, getCallback, NULL, cmd, len);
        // });
        // uv_async_send(async);

        while(1){
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }


    });

    redisLibuvAttach(c2,loop);
    redisAsyncSetConnectCallback(c2,connectCallback);
    redisAsyncSetDisconnectCallback(c2,disconnectCallback);


    uv_run(loop, UV_RUN_DEFAULT);
    return 0;
}