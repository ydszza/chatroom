#ifndef __LISTNODE_H
#define __LISTNODE_H


#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>


//在线列表
typedef struct ListNode_t{
    int sock_fd;//socket描述符
    char account[20];//帐号
    char nick_name[20];//昵称
    time_t last_used;
    struct ListNode_t *next;
} ListNode;

ListNode* add_node_to_list(ListNode** p_head, int sock_fd, char* account, char* nick_name);
int del_list_node(ListNode** p_head, ListNode* p_to_be_deleted);
void print_list(ListNode* p_head);
int find_fd_in_list(ListNode* p_head, char* account);
void list_destroy(ListNode** p_head);



#endif // !__LISTNODE_H