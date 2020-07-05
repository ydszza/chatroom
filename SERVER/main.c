#include "common.h"
#include "server.h"
#include "deal_client.h"
#include <pthread.h>

struct online_list_t online_list;

int main()
{
    online_list.next = NULL;
    int sock_fd=-1;
    sock_fd = server_init(SERVER_ADDR, SERVER_PORT);
    if(sock_fd < 0){
        return -1;
    }
    //开始接收客户端连接
    while(1){
        int client_sock_fd = -1;
        client_sock_fd = server_accept(sock_fd);
        if(client_sock_fd < 0){
            printf("server_accept error\n");
            continue;
        }
        //开启一个线程处理此客户端连接
        pthread_t thread;
        int *fd = (int *)malloc(sizeof(int));
        *fd = client_sock_fd;
        pthread_create(&thread, NULL, (void *)deal_with_client, fd);
    }
    return 0;
}