#define _GNU_SOURCE

#include <string.h>  
#include <stdio.h>  
#include <stdlib.h>  
#include <unistd.h>  
#include <sys/select.h>  
#include <sys/time.h>  
#include <sys/socket.h>  
#include <netinet/in.h>  
#include <arpa/inet.h>  
#include <poll.h>

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
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    ERRP(socket_fd <= 0, return -1, "socket");

    struct sockaddr_in address;
    bzero(&address, sizeof(address));
    address.sin_family = AF_INET;
    inet_pton(AF_INET, ip, &address.sin_addr);
    address.sin_port = htons(port);
    int ret = bind(socket_fd, (struct sockaddr* )&address, sizeof(address));
    ERRP(ret < 0, goto ERR1, "connect");

    ret = listen(socket_fd, 5);
    ERRP(ret < 0, goto ERR1, "listen");

    struct sockaddr_in client;
    socklen_t client_addrlen = sizeof(client);
    printf("Wait guest...\n\n");
    int connfd = accept(socket_fd, (struct sockaddr* )&client, &client_addrlen);
    ERRP(connfd < 0, goto ERR1, "accept");
    printf("connect success: %s:%d\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));

    char buf[BUFSIZE] = {};
    unsigned int cnt = 0;

    struct pollfd fds;
    fds.fd = connfd;
    fds.events = (POLLIN | POLLRDHUP | POLLPRI);

    while (1)
    {

        fds.revents = 0;

        ret = poll(&fds, 1, 4000);
        ERRP(ret < 0, goto ERR2, "poll");

        //注意先判断对端退出事件
        if (fds.revents & POLLRDHUP)
        {

            //printf("exit\n");
            break;
        }
        else if (fds.revents & POLLIN)
        {
           bzero(buf, BUFSIZE);
           ret = recv(connfd, buf, sizeof(buf) - 1, 0);
           ERRP(ret < 0, goto ERR2, "recv normal data");
           printf("get %d bytes of normal data: %s\n", ret, buf);
        }
        else if (fds.revents & POLLPRI)
        {
            bzero(buf, BUFSIZE);
            ret = recv(connfd, buf, sizeof(buf) - 1, MSG_OOB);
            ERRP(ret < 0, goto ERR2, "recv OOB data");
            printf("get %d bytes of OOB data: %s\n", ret, buf);
            //printf ("fds.revents = %d, POLLPRI | POLLRDHUP\n", fds.revents);
        }

        if (ret == 0)    
        {
             printf ("Time out: %d\n", ++cnt);
        }   
    }

ERR2:
    close(connfd);
ERR1:
    close(socket_fd);  

    return 0;
}