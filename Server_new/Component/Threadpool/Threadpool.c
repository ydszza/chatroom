#include "Threadpool.h"

/**
 * 功能：初始化线程池
 * 参数：thread_max_num:最大线程数
 *      tasks_max_num:任务队列存储最大任务量
 * 返回值：成功：线程池地址，失败：NULL
*/
struct threadpool* threadpool_create(int thread_max_num, int thread_min_num, int tasks_max_num)
{
    //检查参数是否有效
    if(thread_max_num < thread_min_num || thread_min_num < 0 || tasks_max_num < 0){
        printf("param is NULL in %s\n", __func__);
        return NULL;
    }
    if(thread_max_num > THREAD_MAX_NUM){
        thread_max_num = THREAD_MAX_NUM;
    }
    if(thread_min_num < THREAD_MIN_NUM){
        thread_min_num = THREAD_MIN_NUM;
    }
    if(tasks_max_num > TASKS_MAX_NUM){
        tasks_max_num = TASKS_MAX_NUM;
    }
    struct threadpool* pool = NULL;
    pool = (struct threadpool*)malloc(sizeof(struct threadpool));
    if(pool == NULL){
        printf("malloc threadpool failed in %s\n", __func__);
        goto ERR_POOL;
    }
    //初始化任务队列
    pool->tasks_max_num = tasks_max_num;
    pool->tasks_cur_num = 0;
    pool->p_tasks_head = NULL;
    pool->p_tasks_tail = NULL;

    //初始化线程参数
    pool->thread_max_num = thread_max_num;
    pool->thread_min_num = thread_min_num;
    pool->thread_alive_num = thread_min_num;
    pool->thread_busy_num = 0;
    pool->thread_wait_exit_num = 0;

    //默认打开线程池
    pool->shutdown = 0;

    //初始化锁和条件变量
    if(pthread_mutex_init(&(pool->mutex_pool), NULL)){
        printf("mutex_pool init failed in %s\n", __func__);
        goto ERR_MUTEX_POOL;
    }
    if(pthread_mutex_init(&(pool->mutex_busy_thread_num), NULL)){
        printf("mutex_busy_thread_num init failed in %s\n", __func__);
        goto ERR_MUTEX_BUSY_THREAD_NUM;
    }
    if(pthread_cond_init(&(pool->tasks_not_empty), NULL)){
        printf("tasks cond init failed in %s\n", __func__);
        goto ERR_TASKS_NOT_EMPTY;
    }
    if(pthread_cond_init(&(pool->tasks_not_full), NULL)){
        printf("tasks cond init failed in %s\n", __func__);
        goto ERR_TASKS_NOT_FULL;
    } 

    //初始化工作线程，个数为线程池线程最小值
    pool->p_threads = (pthread_t*)malloc(sizeof(pthread_t)* thread_max_num);
    if(pool->p_threads == NULL){
        printf("p_threads malloc failed in %s\n", __func__);
        goto ERR_MALLOC_P_THREADS;
    }
    memset((pool->p_threads), 0, sizeof(pthread_t)*thread_max_num);
    int i;
    for(i = 0; i < (pool->thread_min_num); ++i){
        if(pthread_create(&(pool->p_threads[i]), NULL, threadpool_func, (void *)pool)){
            printf("work thread create failed in %s\n", __func__);
            goto ERR_CREATE_THREAD;
        }
        //printf("start thread 0x%lx...\n", pool->p_threads[i]);
    }
    
    //初始化管理者线程
    if(pthread_create(&(pool->thread_manage_id), NULL, threadpool_manage, (void *)pool) != 0){
        printf(" thread_admin create failed in %s\n", __func__);
        goto ERR_CREATE_MANAGE_THREAD;
    }
    //printf("start thread manage 0x%lx...\n", pool->thread_manage_id);

    printf("\nthreadpool create successful!\n");
    return pool;

ERR_CREATE_MANAGE_THREAD:
ERR_CREATE_THREAD:
    pthread_mutex_lock(&(pool->mutex_pool));
    pool->shutdown = 1;
    pthread_mutex_unlock(&(pool->mutex_pool));
    pthread_cond_broadcast(&(pool->tasks_not_empty));
    //等待已创建的线程退出
    for(int j = 0; j < i; ++j) {
        pthread_join(pool->p_threads[j], NULL);
    }
    free(pool->p_threads);
ERR_MALLOC_P_THREADS:
    pthread_cond_destroy(&(pool->tasks_not_full));
ERR_TASKS_NOT_FULL:
    pthread_cond_destroy(&(pool->tasks_not_empty));
ERR_TASKS_NOT_EMPTY:
    pthread_mutex_destroy(&(pool->mutex_busy_thread_num));
ERR_MUTEX_BUSY_THREAD_NUM:
    pthread_mutex_destroy(&(pool->mutex_pool));
ERR_MUTEX_POOL:
    free(pool);
ERR_POOL:
    return NULL;
}

/**
 * 功能：向线程池添加任务
 * 参数：pool:线程池地址
 *      callback：回调函数
 *      arg：添加的任务的地址
 * 返回值：成功：0，失败：-1
*/
int threadpool_add_task(struct threadpool* pool, void* (*callback)(void *arg), void* arg)
{
    if(!pool || !arg || !callback){
        printf("param is NULL in %s\n", __func__);
        return -1;
    }
    //printf("start add task\n");
    //pool上锁
    pthread_mutex_lock(&(pool->mutex_pool));
    //当任务队列满且线程池未关闭,阻塞
    //唤醒条件为任务队列不为满或线程池准备关闭
    while((pool->tasks_cur_num == pool->tasks_max_num) && !(pool->shutdown)){
        pthread_cond_wait(&(pool->tasks_not_full), &(pool->mutex_pool));
    }

    //当线程池准备关闭唤醒时，线程退出
    if(pool->shutdown){
        pthread_mutex_unlock(&(pool->mutex_pool));
        return -1;
    }

    //任务队列不为满，添加任务
    struct task* new_task = (struct task*)malloc(sizeof(struct task));
    if(new_task == NULL){
        pthread_mutex_unlock(&(pool->mutex_pool));
        printf("new_task malloc failed in %s\n", __func__);
        return -1;
    }
    new_task->callback = callback;
    new_task->arg = arg;
    new_task->p_next = NULL;
    //添加任务前任务队列为空
    if(pool->p_tasks_head == NULL){
        pool->p_tasks_head = pool->p_tasks_tail = new_task;
        pthread_cond_signal(&(pool->tasks_not_empty));
    }else{
        pool->p_tasks_tail->p_next = new_task;
        pool->p_tasks_tail = new_task;
    }
    pool->tasks_cur_num++;
    pthread_mutex_unlock(&(pool->mutex_pool));

    printf("add a task successfully\n");
    return 0;
}

/**
 * 功能：销毁线程池
 * 不考虑任务队列是否为空，直接关闭
 * 参数：线程池地址
 * 返回值：成功：0，失败：-1
*/
int threadpool_destroy(struct threadpool *pool)
{
    if(pool == NULL){
        printf("param is NULL in %s\n", __func__);
        return -1;
    }
    pthread_mutex_lock(&(pool->mutex_pool));

    //当线程池已经退出
    if(pool->shutdown){
        pthread_mutex_unlock(&(pool->mutex_pool));
        return -1;
    }
    
    //置线程池为关闭状态
    pool->shutdown = 1;  

    //解锁
    pthread_mutex_unlock(&(pool->mutex_pool));
    
    //唤醒所有线程
    pthread_cond_broadcast(&(pool->tasks_not_empty));
    pthread_cond_broadcast(&(pool->tasks_not_full));
    
    //等待所有线程退出
    for(int i = 0; (i < pool->thread_max_num) && (pool->thread_alive_num > 0); ++i) {
        if(pool->p_threads[i] != 0){
            pthread_join(pool->p_threads[i], NULL);
            pool->thread_alive_num--;   
            printf("join work thread: 0x%lx\n", pool->p_threads[i]);
        } 
    }
    pthread_join(pool->thread_manage_id, NULL);
    printf("join manage thread: 0x%lx\n", pool->thread_manage_id);
    
    //销毁所有锁和条件变量
    pthread_mutex_destroy(&(pool->mutex_pool));
    pthread_mutex_destroy(&(pool->mutex_busy_thread_num));
    pthread_cond_destroy(&(pool->tasks_not_empty));
    pthread_cond_destroy(&(pool->tasks_not_full));
    
    //释放线程ID数组
    free(pool->p_threads);
    pool->p_threads = NULL;
    
    //释放任务队列
    struct task* p_to_be_deleted = NULL; 
    while(pool->p_tasks_head != NULL){
        p_to_be_deleted = pool->p_tasks_head;
        pool->p_tasks_head = pool->p_tasks_head->p_next;
        free(pool->p_tasks_head);
        p_to_be_deleted = NULL;
    }
    
    //释放线程池数据结构
    free(pool);
    
    printf("destory threadpool successful\n");
    return 0;
}

/**
 * 功能：线程池工作线程函数
 * 参数：arg线程池地址
 * 返回值：成功：0，失败：-1
*/
void* threadpool_func(void* arg)
{
    //检查数据
    if(arg == NULL){
        printf("param is NULL in %s\n", __func__);
        pthread_exit(NULL);
    }
    struct threadpool* pool = (struct threadpool*)arg;
    struct task* p_task = NULL;
    while(1){
        //锁定线程池数据结构获取任务
        pthread_mutex_lock(&(pool->mutex_pool));
        //任务队列为空且线程池为关闭时阻塞在此等待
        //唤醒条件为有任务需要处理/需要减少线程/线程池准备关闭
        while(pool->tasks_cur_num == 0 && !(pool->shutdown)){
            //printf("thread 0x%lx waiting...\n", pthread_self());
            //因条件变量阻塞后，会自动解锁，唤醒后会自动加锁
            pthread_cond_wait(&(pool->tasks_not_empty), &(pool->mutex_pool));

            //清除指定数目的空闲线程，当要结束的线程大于0，结束线程
            if(pool->thread_wait_exit_num > 0) {
                pool->thread_wait_exit_num--;
                //当线程池线程个数大于最小线程数，退出
                if(pool->thread_wait_exit_num > pool->thread_min_num){
                    pthread_t self_thread = pthread_self();
                    //清除线程ID
                    for(int i = 0; i < pool->thread_max_num; ++i){
                        if(self_thread == pool->p_threads[i]){
                            pool->p_threads[i] = 0;
                            break;
                        }
                    }
                    pool->thread_alive_num--;
                    pthread_mutex_unlock(&(pool->mutex_pool));
                    printf("thread 0x%lx exit...\n", self_thread);
                    pthread_exit(NULL);
                }
            }
        }
        //因线程池要关闭唤醒，线程退出
        if(pool->shutdown){
            pthread_mutex_unlock(&(pool->mutex_pool));
            pthread_exit(NULL);
        }

        //从任务队列获取任务
        //当前任务数量减一
        pool->tasks_cur_num--;
        p_task = pool->p_tasks_head;
        if(pool->tasks_cur_num == 0){//当前获取的任务是最后一个任务
            pool->p_tasks_head = pool->p_tasks_tail = NULL;
        }else{//不是最后一个任务
            pool->p_tasks_head = p_task->p_next;
        }

        //当任务获取前是full状态时
        if(pool->tasks_cur_num+1 == pool->tasks_max_num){
            pthread_cond_signal(&(pool->tasks_not_full));
        }

        pthread_mutex_unlock(&(pool->mutex_pool));

        //printf("thread 0x%lx start to work\n", pthread_self());
        pthread_mutex_lock(&(pool->mutex_busy_thread_num));
        pool->thread_busy_num++;
        pthread_mutex_unlock(&(pool->mutex_busy_thread_num));
        //获取完任务，执行任务
        (*(p_task->callback))(p_task->arg);
        //printf("thread 0x%lx end to work\n", pthread_self());
        pthread_mutex_lock(&(pool->mutex_busy_thread_num));
        //忙线程+1
        pool->thread_busy_num--;
        pthread_mutex_unlock(&(pool->mutex_busy_thread_num));

        //释放任务
        free(p_task);
        p_task = NULL;
    }
    return NULL;
}

/**
 * 功能：管理者线程
 *      每10秒检测一次，调整线程数量
 * 参数：线程池地址
 * 返回值：成功：0，失败：-1
*/
void* threadpool_manage(void* arg)
{
    if(!arg){
        printf("param is NULL in %s\n", __func__);
        printf("the %s exit,so cant`t manage the threads\n", __func__);
        pthread_exit(NULL);
    }
    struct threadpool* pool = (struct threadpool*)arg;
    while(!(pool->shutdown)){
        sleep(10);
        pthread_mutex_lock(&pool->mutex_pool);
        int tasks_num = pool->tasks_cur_num;
        int alive_thread_num = pool->thread_alive_num;
        pthread_mutex_unlock(&(pool->mutex_pool));
        pthread_mutex_lock(&(pool->mutex_busy_thread_num));
        int busy_threads_num = pool->thread_busy_num;
        pthread_mutex_unlock(&(pool->mutex_busy_thread_num));

        //检测是否需要创建新线程
        //算法：任务数大于最小线程池数，存活线程小于最大线程数
        if((tasks_num > pool->thread_min_num) && (pool->thread_alive_num < pool->thread_max_num)){
            int add = 0;
            pthread_mutex_lock(&(pool->mutex_pool));
            for(int i = 0; (add < THREAD_STEP) && (i < pool->thread_max_num) && (pool->thread_alive_num < pool->thread_max_num); ++i){
                if(pool->p_threads[i] == 0){
                    if(pthread_create(&(pool->p_threads[i]), NULL, threadpool_func, (void *)pool) != 0){
                        printf("work thread create failed in %s\n", __func__);
                        break;
                    }
                    add++;
                    pool->thread_alive_num++;
                }
            }
            pthread_mutex_unlock(&(pool->mutex_pool));
        }

        //检测是否需要销毁线程
        //算法：忙线程*2 < 存活线程  且  存活线程 > 最小线程数
        //随机销毁 THREAD_STEP 个线程，销毁步骤在线程函数中已实现
        if((busy_threads_num*2 < alive_thread_num) && (alive_thread_num > pool->thread_min_num)){
            pthread_mutex_lock(&(pool->mutex_pool));
            pool->thread_wait_exit_num = THREAD_STEP;
            pthread_mutex_unlock(&(pool->mutex_pool));
            for(int i = 0; i < THREAD_STEP; ++i){
                pthread_cond_signal(&(pool->tasks_not_empty));
            }
        }
    }
    return NULL;
}

#if 0
/*test code*/
void* callback_function(void * arg)
{
    if(!arg){
        printf("param is NULL in %s\n", __func__);
        return NULL;
    }
    char* s = (char*)arg;
    printf("doing task print: %s\n", s);
    return NULL;
}

void test(char* test_name)
{
    if(!test_name){
        printf("param is NULL in %s\n", __func__);
        return;
    }
    printf("%s\n", test_name);
    struct threadpool* pool = threadpool_create(20, 5, 100);
    do
    {
        if(!pool){
            printf("threadpool_create failed\n");
            break;
        }
        if(threadpool_add_task(pool, callback_function, "aaa") == -1){
            printf("threadpool_add_task failed\n");
            break;
        }
        sleep(1);
        if(threadpool_add_task(pool, callback_function, "bbb") == -1){
            printf("threadpool_add_task failed\n");
            break;
        }
        sleep(1);
        if(threadpool_add_task(pool, callback_function, "ccc") == -1){
            printf("threadpool_add_task failed\n");
            break;
        }
        sleep(1);
        if(threadpool_add_task(pool, callback_function, "ddd") == -1){
            printf("threadpool_add_task failed\n");
            break;
        }
        sleep(1);
    } while (0);
    threadpool_destroy(pool);
}

int main(void)
{
    test("test1:");
    return 0;
}

#endif