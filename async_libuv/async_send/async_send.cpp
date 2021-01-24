#include <iostream>
#include <thread>
#include <vector>
#include <uv.h>

uv_loop_t* loop;


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
    std::this_thread::sleep_for(std::chrono::seconds(1));
    int i = 0;
    int ret;
    for(i = 0 ; i < 10000 ; ++i){ // 10 ~ 10000
        uv_async_t * async = (uv_async_t *)malloc(sizeof(uv_async_t));
        ret = uv_async_init(loop, async, asyncCallback2);
        
        if(ret != 0) {
            printf("uv_async_init ret:%d\n", ret);
            exit(0);
        }
        ret = uv_async_send(async);
        if(ret != 0) {
            printf("uv_async_send ret:%d\n", ret);
            exit(0);
        }
    }

}
int main(int argc, char * argv[]){
    
    // loop = uv_loop_new();
    // uv_loop_init(loop);


    loop = uv_default_loop();
    

    uv_timer_t * timer = (uv_timer_t *)malloc(sizeof(uv_timer_t));
    uv_timer_init(loop, timer);
    uv_timer_start(timer, timer_cb, 1000, 1000);
    
    pthread_t pid;
    pthread_create(&pid, NULL, thread_f, NULL);

    uv_run(loop, UV_RUN_DEFAULT);

    return 0;
}