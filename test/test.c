#include <stdlib.h>
#include <string.h>
#include <stdio.h>
//在线列表
typedef struct ListNode_t{
    int m_value;
    struct ListNode_t *p_next;
} ListNode;
/*创建节点*/
ListNode* create_list_node(int value)
{
    ListNode* p_newnode = (ListNode*) malloc(sizeof(ListNode));
    memset(p_newnode, 0, sizeof(ListNode));
    p_newnode->m_value = value;
    p_newnode->p_next = NULL;
    return p_newnode;
}

/*添加节点*/
int add_node_to_list(ListNode* p_head, int value)
{
    ListNode* p_newnode = create_list_node(value);
    if(p_newnode == NULL){
        return -1;
    }
    //当链表为空时
    if(p_head == NULL){
        p_head = p_newnode;
    }else{
        //当链表非空
        ListNode* p_temp_node = p_head;
        p_head = p_newnode;
        p_head->p_next = p_temp_node;
    }
    return 0;
}

/*删除节点*/
int del_list_node(ListNode** p_head, ListNode* p_to_be_deleted)
{
    if(!p_head || !p_to_be_deleted){
        printf("list is empty or inexistent\n");
        return -1;
    }
    //要删除的不是尾结点
    if(p_to_be_deleted->p_next != NULL){
        ListNode* p_next = p_to_be_deleted->p_next;
        p_to_be_deleted->m_value = p_next->m_value;
        p_to_be_deleted->p_next = p_next->p_next;
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
        while(p_node->p_next != p_to_be_deleted){
            p_node = p_node->p_next;
        }
        p_node->p_next = NULL;
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
        printf("value = %d\n", p_node->m_value);
        p_node = p_node->p_next;
    }
    printf("show list node end\n");
}

#if 1
/*test code*/
void test(char* test_name, ListNode* p_head)
{
    add_node_to_list(p_head,  1);
    add_node_to_list(p_head, 2);
    add_node_to_list(p_head, 3);
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
    ListNode* head = ;
    test("test1", head);
    return 0;
}
#endif