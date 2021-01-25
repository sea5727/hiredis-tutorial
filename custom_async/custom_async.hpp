#pragma once

#include <hiredis/hiredis.h>
#include <hiredis/async.h>
#include <hiredis/adapters/libuv.h>
#include "EventCLoop.hpp"

typedef
struct
{
    redisAsyncContext *context;
    EventCLoop::Epoll * epoll;
    int mydata;
} RedisEvent;

static 
void 
redisPoll(
    const epoll_event & ev,
    void *privdata) {

    RedisEvent* e = (RedisEvent*)privdata;
    redisContext *c = &e->context->c;
    EventCLoop::Epoll * epoll = e->epoll;

    printf("redisPoll e:%p, c:%p\n", e, c);

    if(e->context == NULL) return;

    
    auto error = EventCLoop::Error{};
    if(ev.events & EPOLLERR){
        std::cout << "[CONNECT] ERROR ? \n";
        error = EventCLoop::Error{strerror(errno)};
    }
    else if(ev.events & EPOLLIN){ // already connected ?? 
        std::cout << "[CONNECT] EPOLLIN ? \n";
        redisAsyncHandleRead(e->context);
    }
    else if(ev.events & EPOLLOUT){
        // epoll->DelEvent(ev.data.fd);
        printf("epollout\n");
        redisAsyncHandleWrite(e->context);
    }
    else{
        epoll->DelEvent(ev.data.fd);
        printf("success\n");
        //callback
    }
}


static 
void 
redisAddWrite(void *privdata) {
    RedisEvent* e = (RedisEvent*)privdata;
    redisContext *c = &(e->context->c);
    EventCLoop::Epoll * epoll = e->epoll;

    printf("redisAddWrite e->context->c:%p\n", &e->context->c);

    using std::placeholders::_1;
    auto event = EventCLoop::Event{};
    event.fd = c->fd;
    event.pop = std::bind(redisPoll, _1, e);
    struct epoll_event ev;
    ev.data.fd = event.fd;
    ev.events = EPOLLOUT;

    epoll->ModEvent(event.fd, ev);
}

static
void
redisDelWrite(void *privdata){

    RedisEvent* e = (RedisEvent*)privdata;
    redisContext *c = &e->context->c;
    EventCLoop::Epoll * epoll = e->epoll;

    printf("redisDelWrite e:%p, c:%p\n", e, c);


    epoll->DelEvent(c->fd);
}

static
void
redisAddRead(void *privdata){
    RedisEvent* e = (RedisEvent*)privdata;
    redisContext *c = &e->context->c;
    EventCLoop::Epoll * epoll = e->epoll;

    printf("redisAddRead e:%p, c:%p\n", e, c);

    using std::placeholders::_1;
    auto event = EventCLoop::Event{};
    event.fd = c->fd;
    event.pop = std::bind(redisPoll, _1, e);
    struct epoll_event ev;
    ev.data.fd = event.fd;
    ev.events = EPOLLIN;

    epoll->AddEvent(event, ev);
}

static
void
redisDelRead(void *privdata){
    RedisEvent* e = (RedisEvent*)privdata;
    redisContext *c = &e->context->c;
    EventCLoop::Epoll * epoll = e->epoll;

    printf("redisDelRead e:%p, c:%p\n", e, c);


    epoll->DelEvent(c->fd);

}




static
int
redisAttach(redisAsyncContext * ac, EventCLoop::Epoll * epoll){
    redisContext *c = &(ac->c);

    /* Nothing should be attached when something is already attached */
    if (ac->ev.data != NULL)
        return REDIS_ERR;

    RedisEvent * e = (RedisEvent *)malloc(sizeof(RedisEvent));
    if (e == NULL)
        return REDIS_ERR;
    
    e->context = ac;
    e->epoll = epoll;

    ac->ev.addRead = redisAddRead;
    ac->ev.delRead  = redisDelRead;
    ac->ev.addWrite  = redisAddWrite;
    ac->ev.delWrite = redisDelWrite;
    ac->ev.cleanup = NULL;
    ac->ev.data = e;


    using std::placeholders::_1;
    auto event = EventCLoop::Event{};
    event.fd = c->fd;
    event.pop = std::bind(redisPoll, _1, e);
    struct epoll_event ev;
    ev.data.fd = event.fd;
    ev.events = EPOLLIN | EPOLLOUT;

    epoll->AddEvent(event, ev);


    return 0;

    //epoll 을 어떻게?
}