#include <iostream>
#include <thread>
#include <hiredis/hiredis.h>

int main(int, char**) {
    
    auto context = redisConnect("127.0.0.1", 6379);
    if(context == NULL | context-> err) {
        if(context){
            fprintf(stderr, "Error : %s\n", context->errstr );
        } else {
            printf("Can't allocate redis context\n");
        }
    }

    redisReply *reply;
    redisAppendCommand(context, "SET foo11 barrr");
    redisAppendCommand(context, "SET foo12 barrr1");
    redisAppendCommand(context, "SET foo23 barrr2");
    redisAppendCommand(context, "SET foo34 barrr3");
    redisAppendCommand(context, "SET foo45 barrr4");

    int a;
    std::cin >> a;

    redisReply * reply1;
    redisReply * reply2;
    redisReply * reply3;
    redisReply * reply4;
    redisReply * reply5;
    redisGetReply(context, (void **)&reply1);
    std::cin >> a;
    redisGetReply(context, (void **)&reply2);
    std::cin >> a;
    redisGetReply(context, (void **)&reply3);
    std::cin >> a;
    redisGetReply(context, (void **)&reply4);
    std::cin >> a;
    redisGetReply(context, (void **)&reply5);
    std::cin >> a;

    std::cout << reply1->integer << ", " << reply1->str << std::endl;
    std::cout << reply1->integer << ", " << reply1->str << std::endl;
    std::cout << reply1->integer << ", " << reply1->str << std::endl;
    std::cout << reply1->integer << ", " << reply1->str << std::endl;
    std::cout << reply1->integer << ", " << reply1->str << std::endl;


    while(1){
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}
