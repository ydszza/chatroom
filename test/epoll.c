#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <unistd.h>

#define SERV_PORT  8802


int main()
{
    int i,flag;
    int sockfd,clntfd,newfd;
    int epfd,nfds;
    ssize_t n;
    char buffer[1024];
    int s = sizeof(struct sockaddr);

    struct sockaddr_in serv_addr;
    struct sockaddr_in clnt_addr;
    //定义epoll数据结构
    struct epoll_event ev,events[20];

    epfd = epoll_create(256);

    //创建socket,并初始化事件ev
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("socket error!\n");
        return -1;
    }
    ev.data.fd = sockfd;
    ev.events = EPOLLIN|EPOLLET;

    //注册epoll事件
    flag = epoll_ctl(epfd, EPOLL_CTL_ADD, sockfd, &ev);
    if (flag < 0) {
        perror("epoll_ctl error!\n");
        return -1;
    }

    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERV_PORT);
    serv_addr.sin_addr.s_addr = htonl( INADDR_ANY );

    flag = bind(sockfd, (struct sockaddr*)&serv_addr, sizeof(struct sockaddr));
    if (flag < 0) {
        perror("bind error!\n");
        return -1;
    }
    printf("bind\n");

    flag = listen(sockfd, 20);
    if (flag < 0) {
        perror("listen error!\n");
        return -1;
    }
    printf("listen\n");

    //开始循环
    while (1) {
        //等待事件发生,返回请求数目
        nfds = epoll_wait(epfd, events, 20, 500);
        //一次处理请求
        for (i = 0; i < nfds; ++i) {
            if (events[i].data.fd == sockfd){
                clntfd = accept(sockfd, (struct sockaddr*)&clnt_addr,(unsigned int*)&s);
                if (clntfd < 0) {
                    perror("accept error");
                    continue;
                }
                printf("accept\n");

                char *str = inet_ntoa(clnt_addr.sin_addr);
                printf("accepnt the client ip : %s\n",str);

                //设置文件标识符，设置操作属性：写操作
                ev.data.fd = clntfd;
                ev.events = EPOLLOUT | EPOLLET;
                //向创建的的epoll进行注册写操作
                epoll_ctl(epfd, EPOLL_CTL_ADD, clntfd, &ev);
            } else if (events[i].events & EPOLLOUT) {
                printf("EPOLLOUT\n");

                if ((newfd = events[i].data.fd) < 0)
                    continue;
                bzero(buffer,sizeof(buffer));
                strcpy(buffer,"welcome to myserver!\n");
                flag = send(newfd, buffer, 1024, 0);
                if (flag < 0) {
                    perror("send error");
                    continue;
                }
                //修改操作为读操作
                ev.data.fd = clntfd;
                ev.events = EPOLLIN | EPOLLET;
                epoll_ctl(epfd, EPOLL_CTL_MOD, newfd, &ev);
            } else if (events[i].events & EPOLLIN) {
                printf("EPOLLIN\n");

                bzero(buffer,sizeof(buffer));
                if ((newfd = events[i].data.fd) < 0)
                    continue;
                if ((n = read(newfd, buffer, 1024)) < 0) {
                    if (errno == ECONNRESET){
                        close(newfd);
                        events[i].data.fd = -1;
                        printf("errno ECONRESET!\n");
                    } else {
                        perror("readbuffer error!\n");
                    }
                } else if (n == 0) {//表示客户端已经关闭
                    close(newfd);
                    events[i].data.fd = -1;
                    printf("n为0\n");
                }
                if (buffer[0] != '0')
                    printf("have read: %s\n", buffer);
            }
        }
    }
    close(sockfd);
    return 0;
}


