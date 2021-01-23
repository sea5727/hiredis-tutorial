# hiredis async


### connect

### reconnect

connectCallback에서 staus가 REDIS_OK가 아닌경우 연결을 재시도한다.
이때 redisAyncConext handle을 해제시켜주지 않아도 내부적으로 해제가 된다.
```cpp
void
connectCallback(
    const redisAsyncContext * conn,
    int status) {
    
    
    if(status != REDIS_OK){ // connnect
        printf("connectCallback conn:%p, err:%d, errstr:%s, status:%d\n", conn, conn->err, conn->errstr, status);
        
        std::this_thread::sleep_for(std::chrono::milliseconds(30));
        c2 = redisAsyncConnect("192.168.0.35", 6380);

        redisLibuvAttach(c2, loop);
        redisAsyncSetConnectCallback(c2, connectCallback);
        redisAsyncSetDisconnectCallback(c2, disconnectCallback);
        return;
    }
    printf("connection success\n");
}

```


### command

