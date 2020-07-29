#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <libgen.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/select.h>

#define ERRP(con, ret, ...) do                              \
{                                                           \
    if (con)                                                \
    {                                                       \
        perror(__VA_ARGS__);                                \
        ret;                                                \
    }                                                       \
}while(0)

#define BUFSIZE 1024
static const char* ip = "192.168.239.136";
static int port = 9660;

int main(void)
{
    //创建套接字
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    ERRP(socket_fd <= 0, return -1, "socket");
    //绑定端口
    struct sockaddr_in address;
    bzero(&address, sizeof(address));
    address.sin_family = AF_INET;
    inet_pton(AF_INET, ip, &address.sin_addr);
    address.sin_port = htons(port);
    int ret = bind(socket_fd, (struct sockaddr* )&address, sizeof(address));
    ERRP(ret < 0, goto ERR1, "connect");
    //监听
    ret = listen(socket_fd, 5);
    ERRP(ret < 0, goto ERR1, "listen");
    //等待客户端连接
    struct sockaddr_in client;
    socklen_t client_addrlen = sizeof(client);
    printf("Wait guest...\n\n");
    int connfd = accept(socket_fd, (struct sockaddr* )&client, &client_addrlen);
    ERRP(connfd < 0, goto ERR1, "accept");
    printf("connect success: %s:%d\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));
    //有客户端连接
    char buf[BUFSIZE] = {};//接收缓存
    fd_set read_fds;
    fd_set exception_fds;
    FD_ZERO(&read_fds);
    FD_ZERO(&exception_fds);
    struct timeval timeout;
    unsigned int cnt = 0;

    while (1)
    {
        bzero(buf, BUFSIZE);

        //select()的参数在每次select()函数的返回会被内核修改，所以这里需要重新设置
        FD_SET(connfd, &read_fds);      //将connfd加入就绪读监听集合
        FD_SET(connfd, &exception_fds); //将connfd加入异常监听集合
        timeout.tv_usec = 0;            //超时时间为4s
        timeout.tv_sec = 4;

        ret = select(connfd + 1, &read_fds, NULL, &exception_fds, &timeout);
        ERRP(ret < 0, goto ERR2, "select");
        //检测connfd是否被置位
        if (FD_ISSET(connfd, &read_fds))
        {
            //读取数据
            ret = recv(connfd, buf, sizeof(buf) - 1, 0);    //recv返回0表示对端已经关闭
            ERRP(ret < 0, goto ERR2, "recv normal data");
            if (ret == 0)
            {
                printf("guest exit...\n");
                break;
            }
            printf("get %d bytes of normal data: %s\n", ret, buf);
        }
        else if (FD_ISSET(connfd, &exception_fds))
        {
            ret = recv(connfd, buf, sizeof(buf) - 1, MSG_OOB);
            ERRP(ret < 0, goto ERR2, "recv OOB data");
            if (ret == 0)
            {
                printf("guest exit...\n");
                break;
            }
            printf("get %d bytes of OOB data: %s\n", ret, buf);
        }
        else if (ret == 0)  //select()返回0表示超时返回
        {
            printf("time out %d\n", ++cnt);
        }
    }

ERR2:
    close(connfd);
ERR1:
    close(socket_fd);  

    return 0;
}

//https://blog.csdn.net/qq_29344757/article/details/78775948?utm_medium=distribute.pc_relevant.none-task-blog-BlogCommendFromMachineLearnPai2-3.compare&depth_1-utm_source=distribute.pc_relevant.none-task-blog-BlogCommendFromMachineLearnPai2-3.compare