#include "ListNode.h"

/*创建节点*/
ListNode* create_list_node(int socket, char* account, char* nick_name)
{
    if(socket < 0 || account == NULL || nick_name == NULL){
        printf("invalid data\n");
        return NULL;
    }
    ListNode* p_newnode = (ListNode*) malloc(sizeof(ListNode));
    memset(p_newnode, 0, sizeof(ListNode));
    p_newnode->sock_fd = socket;
    strcpy(p_newnode->account, account);
    strcpy(p_newnode->nick_name, nick_name);
    p_newnode->last_used = time(NULL); 
    p_newnode->next = NULL;
    return p_newnode;
}

/*添加节点*/
ListNode* add_node_to_list(ListNode** p_head, int sock_fd, char* account, char* nick_name)
{
    ListNode* p_newnode = create_list_node(sock_fd, account, nick_name);
    if(p_newnode == NULL){
        return NULL;
    }
    
    //当链表为空时
    if((*p_head) == NULL){
        (*p_head) = p_newnode;
    }else{
        //当链表非空
        ListNode* p_temp_node = (*p_head);
        (*p_head) = p_newnode;
        (*p_head)->next = p_temp_node;
    }
    return p_newnode;
}

/*删除节点*/
int del_list_node(ListNode** p_head, ListNode* p_to_be_deleted)
{
    if(!p_head || !p_to_be_deleted){
        printf("list is empty or inexistent\n");
        return -1;
    }
    //要删除的不是尾结点
    if(p_to_be_deleted->next != NULL){
        ListNode* p_next = p_to_be_deleted->next;
        p_to_be_deleted->sock_fd = p_next->sock_fd;
        strcpy(p_to_be_deleted->account, p_next->account);
        strcpy(p_to_be_deleted->nick_name, p_next->nick_name);
        p_to_be_deleted->last_used = p_next->last_used;
        p_to_be_deleted->next = p_next->next;
        free(p_next);
        p_next = NULL;
    }else if (*p_head == p_to_be_deleted){
        //只有一个节点
        free(p_to_be_deleted);
        p_to_be_deleted = NULL;
        *p_head = NULL;
    }else{
        //删除尾结点
        ListNode* p_node = *p_head;
        while(p_node->next != p_to_be_deleted){
            p_node = p_node->next;
        }
        p_node->next = NULL;
        free(p_to_be_deleted);
        p_to_be_deleted = NULL;
    }
    return 0;
}

/*打印链表*/
void print_list(ListNode* p_head)
{
    ListNode* p_node = p_head;
    printf("\nshow list node start\n");
    while(p_node != NULL){
        printf("sock_fd = %d\t account = %s\t nick_name = %s\n",
                p_node->sock_fd, p_node->account, p_node->nick_name);
        p_node = p_node->next;
    }
    printf("show list node end\n");
}

/**
 * 在列表中查找节点并执行对应操作
 * 成功返回0,失败-1
 */
int find_to_dosomething(ListNode* p_head, char* account, void* (*callback)(void * arg))
{
    if(p_head == NULL || account == NULL || callback == NULL){
        return -1;
    }
    ListNode* p_node = p_head;
    while(p_node != NULL){
        if(strcmp(p_node->account, account) == 0){
            (*callback)(&(p_node->sock_fd));
            return 0;
        }
    }
    return -1;
}

/**
 * 遍历列表执行操作
 * 成功返回0,失败-1
*/
int traversing_list_dosomething(ListNode* p_head, void* (*callback)(void * arg))
{
    if(!p_head || !callback){
        return -1;
    }
    ListNode* p_node = p_head;
    while(p_node != NULL){
        (*callback)(&(p_node->sock_fd));
    }
    return 0;
}
#if 0
/*test code*/
void test(char* test_name, ListNode* p_head)
{
    add_node_to_list(&p_head,  1, "122335", "sdfds");
    add_node_to_list(&p_head, 2, "122335", "sdfds");
    add_node_to_list(&p_head, 3, "123455", "acvbb");
    if(test_name == NULL || p_head == NULL){
        printf("invalid input\n");
        return;
    }
    printf("%s:\n", test_name);
    print_list(p_head);
    del_list_node(&p_head, p_head);
    print_list(p_head);
    del_list_node(&p_head, p_head);
    print_list(p_head);
    del_list_node(&p_head, p_head);
    print_list(p_head);
    del_list_node(&p_head, p_head);
    print_list(p_head);
    del_list_node(&p_head, p_head);
    print_list(p_head);
}


int main(int argc, char *argv[])
{    
    ListNode* head = NULL;
    test("test1", head);
    return 0;
}
#endif