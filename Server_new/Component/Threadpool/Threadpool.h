#ifndef __THREADPOOL_H
#define __THREADPOOL_H

#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "../../Project/Config.h"

/*线程池任务数据结构*/
struct task 
{
    void* (*callback)(void* arg);   //callback为任务的对应处理方法
    void* arg;                      //arg为callback的参数
    void* p_next;                   //指向下一个任务
};

/*线程池数据结构*/
struct threadpool
{
    int tasks_max_num;              //线程池任务最大数量
    int tasks_cur_num;              //线程池当前任务数量
    struct task* p_tasks_head;      //线程池任务队列头
    struct task* p_tasks_tail;      //线程池任务队列尾

    int thread_max_num;             //线程池最大线程数
    int thread_min_num;             //线程池最小线程数
    int thread_alive_num;           //线程池已经存在的线程数
    int thread_busy_num;            //线程池正在工作的线程数
    int thread_wait_exit_num;       //等待退出的线程个数

    pthread_t* p_threads;           //线程池线程ID集合
    pthread_t thread_manage_id;      //线程池

    pthread_mutex_t mutex_pool;     //线程池数据结构锁
    pthread_mutex_t mutex_busy_thread_num;
    pthread_cond_t tasks_not_empty; //线程池任务队列非空条件变量
    pthread_cond_t tasks_not_full;  //线程池任务队列非满条件变量

    int shutdown;                   //线程池是否关闭(非零即关闭)
};

struct threadpool* threadpool_create(int thread_max_num, int thread_min_num, int tasks_max_num);
int threadpool_add_task(struct threadpool* pool, void* (*callback)(void * arg), void* arg);
int threadpool_destroy(struct threadpool* pool);
void* threadpool_func(void* arg);
void* threadpool_manage(void* arg);


#endif // !__THREADPOOL_H