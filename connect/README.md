
# hiredis connection 관련 정리

### blocking connect
connect 함수는 아주 간단하다.
option을 사용하는게 코드관리에 좋아보인다.


### connection 단절확인

blocking 인 경우 커넥션 확인은 아래와 같다.

```cpp
ret = redisGetReply(c, (void **)&r); // connectio close 감지
if(ret != REDIS_OK){
    printf("redisGetReply(%d) != REDIS_OK \n", ret);
    return;
}
```


