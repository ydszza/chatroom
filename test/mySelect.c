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

#define ERRP(con, ret, ...) do              \
{                                           \
    if(con)                                 \
    {                                       \
        perror(__VA_ARGS__);                \
        ret;                                \
    }                                       \
}while(0)                                   \

#define BUFSIZE   1024
static const char* ip = "127.0.0.1";
static int port = 12345;

int main(int argc, char *argv[])
{
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    ERRP(socket_fd <= 0, return -1, "socket");
    
    struct sockaddr_in address;
    bzero(&address, sizeof(address));
    address.sin_family = AF_INET;
    inet_pton(AF_INET, ip, &address.sin_addr);
    address.sin_port = htons(port);
    int ret = bind(socket_fd, (struct sockaddr* )&address, sizeof(address));
    ERRP(ret < 0, goto ERR1, "bind");

    ret = listen(socket_fd, 5);
    ERRP(ret < 0, goto ERR1, "listen");

    struct sockaddr_in client;
    socklen_t client_addrlen = sizeof(client);
    printf("Wait guest...\n\n");
    int connfd = accept(socket_fd, (struct sockaddr* )&client, &client_addrlen);
    ERRP(connfd < 0, goto ERR1, "accept");
    printf("connect success: %s:%d\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));

    char buf[BUFSIZE] = {0};
    fd_set read_fds;
    fd_set exception_fds;
    FD_ZERO(&read_fds);
    FD_ZERO(&exception_fds);
    struct timeval timeout;
    unsigned int cnt = 0;

    while (1)
    {
        bzero(buf, BUFSIZE);

        FD_SET(connfd, &read_fds);
        FD_SET(connfd, &exception_fds);
        timeout.tv_sec = 4;
        timeout.tv_usec =0;
        ret = select(connfd + 1, &read_fds, NULL, &exception_fds, &timeout);
        ERRP(ret < 0, goto ERP2, "select");

        if(FD_ISSET(connfd, &read_fds))
        {
            ret = recv(connfd, buf, sizeof(buf)-1, 0);
            ERRP(ret < 0, goto ERP2, "RECV");
            if(ret == 0)
            {
                printf("guest exit...\n");
                break;
            }
            printf("guest %d bytes of normal data : %s\n",ret, buf);
        }
        else if (FD_ISSET(connfd, &exception_fds))
        {
            ret = recv(connfd, buf, sizeof(buf) - 1, MSG_OOB);
            ERRP(ret < 0, goto ERP2, "recv OOB data");
            if(ret == 0)
            {
                printf("guest exit...\n");
                break;
            }
            printf("guest %d bytes of OOB data: %s\n", ret, buf);
                
        }
        else if (ret == 0)
        {
            printf("timeout %d\n", ++cnt);
        }
    }

ERP2:
close(connfd);

ERR1:
    close(socket_fd);

    return 0;
}