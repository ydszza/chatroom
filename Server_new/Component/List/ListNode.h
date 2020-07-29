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

ListNode* create_list_node(int socket, char* account, char* nick_name);
ListNode* add_node_to_list(ListNode** p_head, int sock_fd, char* account, char* nick_name);
int del_list_node(ListNode** p_head, ListNode* p_to_be_deleted);
void print_list(ListNode* p_head);
int find_to_dosomething(ListNode* p_head, char* account, void* (*callback)(void * arg));
int traversing_list_dosomething(ListNode* p_head, void* (*callback)(void * arg));



#endif // !__LISTNODE_H