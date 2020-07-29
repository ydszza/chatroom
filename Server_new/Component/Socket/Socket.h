#ifndef __SOCKET_H
#define __SOCKET_H

#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <time.h>
#include "../../Project/Config.h"
#include "../List/ListNode.h"
#include "../UserManage/UserManage.h"

struct fd_info {
    int sock_fd;
    int epoll_fd;
    unsigned int event;
    void* (*callback)(void * arg);
    char* buf;
    int len;
    ListNode* node;
    ListNode** head;
};


/*转发的聊天消息*/
typedef struct message_t {
    unsigned char flag;
    char from_user[20];
    char to_user[20];
    char data[1024];
} message;

/*接收的消息头和回复各类验证消息*/
/*使用 unsigned char flag;*/
/**
 * 从低位到高位
 * 接收时含义，该位为1表示该类型消息，16位中只可能有一个1
 * 注册/登录/修改个人信息/群聊或私聊
 * 发送时含义：该位为1表示操作成功或验证成功，16位中只可能有一个1
 * 注册/登录/个人信息修改
*/

/*接受发送消息类型*/
#define REGISTER                (1<<0)
#define SIGN_IN                 (1<<1) 
#define CHANGE_USERINFO         (1<<2)
#define RECV_MSG                (1<<3)





int socket_init_to_listen(int* epoll_fd, ListNode** head);
void* socket_accept(void* arg);
void* socket_recv(void* arg);
void* socket_send(void* arg);


#endif // !__SOCKET_H