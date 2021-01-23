#include <iostream>
#include <thread>
#include <vector>
#include <hiredis/hiredis.h>

redisContext *c;
void 
queryFormat(){
    redisReply * reply;
    int ret;
    ret = redisAppendCommand(c, "%s", "SET KEY1 VALUE1"); // FAIL 
    std::cout << "ret1:" << ret << std::endl;
    ret = redisGetReply(c, (void **)&reply); // 
    std::cout << "reply-ret1:" << ret << std::endl;        

    ret = redisAppendCommand(c, "%s %s", "SET", "KEY2 VALUE2"); // FAIL 
    std::cout << "ret2:" << ret << std::endl;
    ret = redisGetReply(c, (void **)&reply); // 
    std::cout << "reply-ret2:" << ret << std::endl;

    ret = redisAppendCommand(c, "%s %s", "SET KEY2_2", "VALUE2_2"); // FAIL 
    std::cout << "ret2_2:" << ret << std::endl;
    ret = redisGetReply(c, (void **)&reply); // 
    std::cout << "reply-ret2_2:" << ret << std::endl;    

    ret = redisAppendCommand(c, "%s %s %s", "SET", "KEY3", "VALUE3"); // FAIL 
    std::cout << "ret3:" << ret << std::endl;
    ret = redisGetReply(c, (void **)&reply); // 
    std::cout << "reply-ret3:" << ret << std::endl;

    ret = redisAppendCommand(c, "%s %s%s %s", "SET", "KEY", "3-1",  "VALUE3"); // FAIL 
    std::cout << "ret3:" << ret << std::endl;
    ret = redisGetReply(c, (void **)&reply); // 
    std::cout << "reply-ret3:" << ret << std::endl;

    ret = redisAppendCommand(c, "SET %s %s", "KEY4", "VALUE4"); // SUCCESS
    std::cout << "ret4:" << ret << std::endl;
    ret = redisGetReply(c, (void **)&reply);
    std::cout << "reply-ret4:" << ret << std::endl;
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

    queryFormat();
    
    printf("end\n");
    return 0;
}