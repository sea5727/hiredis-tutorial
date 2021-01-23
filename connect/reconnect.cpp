#include <iostream>
#include <thread>
#include <vector>
#include <hiredis/hiredis.h>

redisContext *c;
redisReply * r;

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


int main(int argc, char * argv[]){

    auto t = std::thread([]{
        int ret = 0;
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

        ret = redisGetReply(c, (void **)&r); // connectio close 감지
        if(ret != REDIS_OK){
            printf("redisGetReply(%d) != REDIS_OK \n", ret);
            return;
        }

    });




    t.join();

    printf("end\n");
    return 0;
}