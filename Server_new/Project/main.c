
#include "../Component/Socket/Socket.h"
#include "../Component/Threadpool/Threadpool.h"
#include "../Component/List/ListNode.h"
#include "../Component/Epoll/Epoll.h"

#define BACK_EVENTS_NUM     50


int main(void) 
{
    //在线列表头结点
    ListNode* online_clients = NULL;
    struct threadpool* pool;
    int epoll_fd;
    int server_fd;
    do{

        //启动线程池
        pool = threadpool_create(50, 10, 50);
        if(!pool){
            break;
        }

        //初始化epoll
        epoll_fd = epoll_init();
        if(epoll_fd < 0){
            break;
        }

        //初始化监听
        server_fd = server_init(epoll_fd, &online_clients);
        if(server_fd < 0){
            break;
        }
        
        //开始循环监听客户端连接和接收客户端消息
        //接收epoll事件的数组
        struct epoll_event back_events[BACK_EVENTS_NUM];
        int is_stop = 0;
        while(!is_stop){
            epoll_wait_events(epoll_fd, back_events, BACK_EVENTS_NUM, pool);
        }
    }while(0);

    //出错
    threadpool_destroy(pool);
    list_destroy(&online_clients);
    close(server_fd);
    close(epoll_fd);
    return 0;
}