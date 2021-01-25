#pragma once

#include "Event.hpp"

namespace EventCLoop{

    class testclass {
        int a;
        std::function<void()> pop;
    };
    
    class Epoll {
        // constexpr static unsigned int EpollSize = 1024;
        // constexpr static unsigned int EpollTimeout = 1024;
        constexpr static unsigned int EpollSize = 65000;
        constexpr static unsigned int EpollTimeout = 65000;
        int epollfd;
        std::array<Event, 65000> allevent;
        // std::array<Event *, 65000> pallevent;
        struct epoll_event ev[EpollSize];
    public:
        Epoll()  {
            std::function<void()> f;

            Error error;
            Init(error);
            if(error)
                throw error;

        }

        Epoll(Error & error) noexcept {
            Init(error);
        }

        inline
        void
        AddEvent(Event & event, struct epoll_event & ev){
            Error error;
            AddEvent(event, ev, error);
            if(error)
                throw error;
        }

        inline
        void
        AddEvent2(Event * event, struct epoll_event & ev, Error & error) noexcept {
        #ifdef DEBUG
            std::cout << "[" << now_str() << "] ========== start Call AddEvent : " << event.fd << std::endl;
        #endif
            if(epoll_ctl(epollfd, EPOLL_CTL_ADD, event->fd, &ev) == -1){
                error = Error{std::string{"epoll_ctl EPOLL_CTL_ADD fail : "} + strerror(errno) + std::to_string(errno)};
                return;
            }
            // pallevent[event->fd] = event;
            // allevent[event.fd] = event;
        }

        inline
        void
        AddEvent(Event & event, struct epoll_event & ev, Error & error) noexcept {
        #ifdef DEBUG
            std::cout << "[" << now_str() << "] ========== start Call AddEvent : " << event.fd << std::endl;
        #endif
            std::cout << " ========== start Call AddEvent : " << event.fd << std::endl;
            if(epoll_ctl(epollfd, EPOLL_CTL_ADD, event.fd, &ev) == -1){
                if(errno == EEXIST){
                    if(epoll_ctl(epollfd, EPOLL_CTL_MOD, event.fd, &ev) == -1){
                        printf("epollfd:%d, event.fd:%d, errno:%d, strerror:%s\n", epollfd, event.fd, errno, strerror(errno));
                        error = Error{std::string{"epoll_ctl EPOLL_CTL_ADD fail : "} + strerror(errno)};
                        return;
                    }
                }
            }
            allevent[event.fd] = event;
        }

        inline
        void
        DelEvent(int eventfd){
            Error error;
            DelEvent(eventfd, error);
            if(error) throw error;
        }

        inline
        void
        DelEvent(int eventfd, Error & error) noexcept {
        #ifdef DEBUG
            std::cout << "[" << now_str() << "] ========== Call DelEvent : " << eventfd << std::endl;
        #endif
            // allevent[eventfd].clear();
            if(epoll_ctl(epollfd, EPOLL_CTL_DEL, eventfd, nullptr) == -1){
                error = Error{std::string{"epoll_ctl EPOLL_CTL_ADD fail : "} + strerror(errno)};
            }
        }

        inline
        void
        ModEvent(int eventfd, struct epoll_event & ev){
            Error error;
            ModEvent(eventfd, ev, error);
            if(error) throw error;
        }

        inline
        void
        ModEvent(int eventfd, struct epoll_event & ev, Error & error) noexcept {
        #ifdef DEBUG
            std::cout << "[" << now_str() << "] ========== Call ModEvent : " << eventfd << std::endl;
        #endif
            if(epoll_ctl(epollfd, EPOLL_CTL_MOD, eventfd, &ev) == -1){
                Error{std::string{"epoll_ctl EPOLL_CTL_MOD fail : "} + strerror(errno)};
            }
        }


        void
        Run(){
            auto count = epoll_wait(epollfd, ev, EpollSize, EpollTimeout);
            for(int i = 0 ; i < count ; ++i){
                // std::cout << "fd:" << ev[i].data.fd << " Epoll event.. event: " << ev[i].events << std::endl;
                int key = ev[i].data.fd;
                auto & event = allevent[key];
                if(event.pop){
                    event.pop(ev[i]);
                }

            }
        }
    private:
        void
        Init(Error & error) noexcept { 
            epollfd = epoll_create1(0);
            if(epollfd == -1){                
                error = Error{std::string{"epoll_create1 : "} + strerror(errno)};
            }
        }


    };



}