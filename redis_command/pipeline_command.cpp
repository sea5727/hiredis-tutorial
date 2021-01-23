#include <iostream>
#include <thread>
#include <vector>
#include <hiredis/hiredis.h>

redisContext *c;

void
pipeline(){
    redisReply * reply;
    int ret;
    for(int i = 0 ; i < 1000 ; ++i){
        ret = redisAppendCommand(c, "%s %s%d %s", "SET", "KEY", i,  "VALUE3"); // FAIL 
        if(ret != REDIS_OK){
            printf("redisAppendCommand fail :%d\n", ret);
            return;
        }
    }
    for(int i = 0 ; i < 1000 ; ++i){
        ret = redisGetReply(c, (void **)&reply);
        if(ret != REDIS_OK){
            printf("redisGetReply fail :%d\n", ret);
            return;
        }
    }
}

void
pipeline_args(){
    redisReply * reply;
    int ret;

    int list_count = 5;
    
    for(int i = 0 ; i < 100000 ; ++i){
        const char * buf2[2 + list_count * 2] = {"ZADD", };
        std::string scores[list_count];
        std::string values[list_count];
        std::string key = std::string("MYKEY") + std::to_string(i);
        buf2[1] = key.c_str();
        for(int j = 0 ; j < list_count ; j++){
            scores[j] = std::to_string(j);
            values[j] = std::string("MYVALUE") + std::to_string(j);
            buf2[2 + j * 2] = scores[j].c_str();
            buf2[2 + j * 2 + 1] = values[j].c_str();
        }

        ret = redisAppendCommandArgv(c, 2 + list_count * 2, buf2, NULL);
        if(ret != REDIS_OK){
            printf("redisAppendCommand fail :%d\n", ret);
            return;
        }
    }

    for(int i = 0 ; i < 100000 ; ++i){
        ret = redisGetReply(c, (void **)&reply);
        if(ret != REDIS_OK){
            printf("redisGetReply fail :%d\n", ret);
            return;
        }
    }    
}



int main(int argc, char * argv[]){
    
    // size_t arr[] = {1,2,3,4};
    // redisCommandArgv(c, argc, (const char **)argv, arr);

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

    pipeline_args();

    printf("end\n");
    return 0;
}