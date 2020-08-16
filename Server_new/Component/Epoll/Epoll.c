#include "Epoll.h"

/**
 * epoll初始化
*/
int epoll_init()
{
    int epoll_fd = epoll_create(EPOLL_MAX_NUM);
    if(epoll_fd < 0){
        printf("epoll_create failed in %s\n", __func__);
        return -1;
    }
    printf("\nepoll_init successful!\n");
    return epoll_fd;
}

/**
 * 初始化event_arg
*/
struct event_arg* event_arg_create(int epoll_fd, int fd, unsigned int event, void* arg, void* (*callback)(void * arg), char* buf, int buf_len)
{
    struct event_arg * new_event = (struct event_arg *)malloc(sizeof(struct event_arg));
    if(!new_event){
        return NULL;
    }
    new_event->epoll_fd = epoll_fd;
    new_event->fd = fd;
    new_event->event = event;
    new_event->arg = arg;
    new_event->callback = callback;
    new_event->buf = buf;
    new_event->buf_len = buf_len;
    new_event->is_chat = 0;
    return new_event;
}

/**
 * 向epoll注册监听的描述符
*/
int epoll_add_fd(int epoll_fd, struct event_arg* arg)
{
       struct epoll_event write_event;
       write_event.events = arg->event | EPOLLET;
       write_event.data.ptr = arg;
       if(epoll_ctl(epoll_fd, EPOLL_CTL_ADD, arg->fd, &write_event) < 0) {
           printf("epoll_add_fd failed\n");
           return -1;
       }
       return 0;
}

/**
 * 从epoll删除注册的描述符
*/
int epoll_del_fd(int epoll_fd, int del_fd) 
{
    if(epoll_ctl(epoll_fd, EPOLL_CTL_DEL, del_fd, NULL) < 0){
        printf("delete fd from epoll failed in %s\n", __func__);
        return -1;
    }
    return 0;
}

/**
 * epoll监听
*/
int epoll_wait_events(int epoll_fd, struct epoll_event* event_table, int max_ev_num, struct threadpool* pool)
{
    int n_ready = epoll_wait(epoll_fd, event_table, max_ev_num, -1);
    if(n_ready < 0){
        printf("epoll_wait failed\n");
        return -1;
    }
    printf("%d events ready\n", n_ready);
    struct event_arg *back_arg = NULL;
    for(int i = 0; i < n_ready; ++i){
        back_arg = (struct event_arg *)(event_table[i].data.ptr);
        //printf("0x%x 0x%x\n", back_arg->event, event_table[i].events);
        if(event_table[i].events == back_arg->event){
            threadpool_add_task(pool, back_arg->callback, back_arg);
            //printf("add task successful\n");
        }
    }
    return 0;
}

