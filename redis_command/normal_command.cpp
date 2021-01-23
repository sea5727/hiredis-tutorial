#include <iostream>
#include <thread>
#include <vector>
#include <hiredis/hiredis.h>

redisContext *c;


void
command(){
    redisReply * reply;
    int ret;
    for(int i = 0 ; i < 1000 ; ++i){
        reply = (redisReply *)redisCommand(c, "%s %s%d %s", "SET", "KEY", i,  "VALUE3"); // FAIL 
        if(reply == nullptr){
            printf("redisAppendCommand fail :%d\n", ret);
            return;
        }
    }
}





int main(int argc, char * argv[]){

    c = redisConnect("127.0.0.1", 6380);
    if (c == NULL || c->err) {
        if (c) {
            printf("Error: %s\n", c->errstr);
            // handle error
        } else {
            printf("Can't allocate redis context\n");
        }
    }
    printf("Redis connect success\n");

    command();

    printf("end\n");
    return 0;
}