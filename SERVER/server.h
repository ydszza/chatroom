#ifndef __SERVER_H
#define __SERVER_H

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>         
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include "common.h"

int server_init(char *server_ip, int server_port);
int server_accept(int sock_fd);
int server_send(int sock_fd, struct send_message_t *send_buf_t);
int server_recv(int sock_fd, struct recv_message_t*recv_buf_t);
int server_close(int sock_fd);


#endif // !__SERVER_H