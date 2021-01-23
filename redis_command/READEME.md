


- hiredis command 호출시 format 은 "%s %s %s" 처럼 공백 규격을 맞춰야 동작한다.

```cpp
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

```

파이프라인
```cpp
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

```
실행시간
```
time ./pipeline_command 
Redis connect success
end

real    0m0.008s
user    0m0.003s
sys     0m0.003s
```


일반 커맨드
```cpp
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
```
실행시간
```
time ./normal_command
Redis connect success
end

real    0m0.159s
user    0m0.008s
sys     0m0.016s
```