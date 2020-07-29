
#include "../Component/Socket/Socket.h"
#include "../Component/Threadpool/Threadpool.h"
#include "../Component/List/ListNode.h"

#define BACK_EVENTS_NUM     50


int main(void) 
{
    //在线列表头结点
    ListNode* online_clients = NULL;

    //启动线程池
    struct threadpool* pool = threadpool_create(50, 10, 50);
    if(!pool){
        goto ERR_CREATE_THREADPOOL;
    }
    
    //初始化监听socket和epoll描述符
    int epoll_fd;
    if((epoll_fd = socket_init_to_listen(&epoll_fd, &online_clients)) < 0){
        printf("socket fd error\n");
        goto ERR_SERVER_FD;
    }
    printf("socket to listen...\n");

    //开始循环监听客户端连接和接收客户端消息
    int nfds;//监听到的事件个数
    //接收epoll事件的数组
    struct epoll_event back_events[BACK_EVENTS_NUM];
    struct fd_info* back_info= NULL;
    int is_stop = 0;
    while(!is_stop){
        nfds = epoll_wait(epoll_fd, back_events, BACK_EVENTS_NUM, -1);
        for(int i = 0; i < nfds; ++i){
            back_info = (struct fd_info *)(back_events[i].data.ptr);
            //添加任务到线程池
            if(threadpool_add_task(pool, back_info->callback, back_info) != 0){
                is_stop = 1;
                break;
            }
        }
    }
    threadpool_destroy(pool);
    close(epoll_fd);
    exit(0);

ERR_SERVER_FD:
    close(epoll_fd);
ERR_EPOLL_CREATE:
    threadpool_destroy(pool);
ERR_CREATE_THREADPOOL:
    exit(1); 
}