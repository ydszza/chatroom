#ifndef __EPOLL_H
#define __EPOLL_H

#include <sys/epoll.h>
#include <stdio.h>
#include "../Threadpool/Threadpool.h"

#define EPOLL_MAX_NUM 1024

struct event_arg {
    int epoll_fd;//epoll描述符
    int fd; //监听的描述符
    unsigned int event;//监听的事件
    void* arg; //范型参数
    void* (*callback)(void * arg);//事件的回调函数
    char* buf;//写入描述符的数据
    int buf_len;//数据的长度
    unsigned char is_chat;//0验证消息，1群聊消息，2私聊消息
};

int epoll_init();
int epoll_add_fd(int epoll_fd, struct event_arg* arg);
struct event_arg* event_arg_create(int epoll_fd, int fd, unsigned int event, void* arg, void* (*callback)(void * arg), char* buf, int buf_len);
int epoll_del_fd(int epoll_fd, int del_fd) ;
int epoll_wait_events(int epoll_fd, struct epoll_event* event_table, int max_ev_num, struct threadpool* pool);


#endif // !__EPOLL_H