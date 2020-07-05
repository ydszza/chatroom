#ifndef __DEAL_CLIENT_H
#define __DEAL_CLIENT_H

#include "common.h"
#include "server.h"
#include <stdlib.h>

extern struct online_list_t online_list;

void deal_with_client(void *arg);
int verify_client(int sock_fd);
void add_member(int sock_fd, char *account);
void delete_member(int sock_fd, char *account);
int send_to_all(enum types type_t, struct send_message_t *msg);
void client_on_or_out_line(char *account, unsigned char flag);
int get_member_sockfd(char *account);
void send_to_allmember(struct send_message_t *msg);
void show_all_members(void);
void sync_online_members_to_client(int sock_fd);


#endif // !__DEAL_CLIENT_H