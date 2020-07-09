#ifndef __COMMON_H
#define __COMMON_H

#define SERVER_ADDR     "192.168.124.5"
#define SERVER_PORT     12345

//接收的消息格式
struct recv_message_t{
    char account[10];
    char password[10];
    unsigned char length[4];
    char data[1024];
};

//发送的消息的格式
struct send_message_t{
    char flag;
    char verify;
    char account[10];
    unsigned char length[4];
    char data[1024];
};

//在线列表
struct online_list_t{
    int sock_fd;
    char account[10];
    int number;
    struct online_list_t *next;
};

//群发消息类型
enum types{
    ON_LINE,
    OUT_LINE,
    SEND_MSG
};



#endif // !__COMMON_H