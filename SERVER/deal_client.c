#include "deal_client.h"

/**
 * 功能：客户端消息转发线程函数
 * 参数arg：客户端socket描述符指针
 * 返回值：无
*/
void deal_with_client(void *arg)
{
    int client_sock_fd = *((int *)arg);
    free(arg);
    arg = NULL;
    printf("\n\n通信线程 %d 开始执行\n", client_sock_fd);

    //验证并添加在线列表再通知所有客户端
    if(verify_client(client_sock_fd) < 0){ //验证失败
        printf("verify error\n");
        server_close(client_sock_fd);
        return;
    }
    //验证成功
    //printf("验证账号密码成功\n");
    //打印所有在线客户端
    show_all_members();
    //同步所有在线客户端
    sync_online_members_to_client(client_sock_fd);

    printf("准备工作结束，开始转发消息\n\n");

    //开始处理客户端请求
    struct recv_message_t recv_msg;
    struct send_message_t send_msg;
    while(1){
        //接收消息
        memset(&recv_msg, 0, sizeof(recv_msg));
        memset(&send_msg, 0, sizeof(send_msg));
        int ret = server_recv(client_sock_fd, &recv_msg);
        //接收消息失败或客户端断开连接
        if(ret <= 0){
            printf("\n\n有人下线了\n");
            //关闭连接
            server_close(client_sock_fd);
            //从在线列表删除
            char account[10];
            //删除该帐号并获取该帐号
            delete_member(client_sock_fd, account);
            //下线通知其他客户端
            memcpy(send_msg.account, account, 10);
            printf("下线帐号：%s\n",send_msg.account);
            send_to_all(OUT_LINE, &send_msg);
            printf("断开连接，结束 %d 通信线程\n", client_sock_fd);
            show_all_members();
            return;
        }
        // printf("account=%s\n", recv_msg.account);
        // printf("data=%s\n", recv_msg.data);
        //转发消息
        send_msg.flag = 2;//类型为消息
        //将发送的消息帐号改为消息来源的帐号
        memcpy(send_msg.account, recv_msg.password, 10);
        memcpy(send_msg.data, recv_msg.data, sizeof(recv_msg.data));
        //判断消息是私聊还是群聊并转发消息
        if(memcmp(recv_msg.account, "0000000000", 10) == 0){//群发消息
            // printf("帐号：%s",send_msg.account);
            // printf("内容：%s",send_msg.data);
            send_msg.verify = 0;//群聊标志
            send_to_all(SEND_MSG, &send_msg);
        }else{//私聊消息
            //查找在线列表找到私聊对象得到对应sock_fd
            int fd = get_member_sockfd(recv_msg.account);
            //发送消息
            // printf("帐号：%s",send_msg.account);
            // printf("内容：%s",send_msg.data);
            send_msg.verify = 1;//私聊标志
            server_send(fd, &send_msg);
        }
        printf("收到一条消息已转发\n");
    }
}

/**
 * 功能：验证账号密码，成功后加入在线列表,并通知其他人
 * 参数：客户端socket描述符
 * 返回值：-1失败，0成功
*/
int verify_client(int sock_fd)
{
    //获取账号密码
    struct recv_message_t recv_msg;
    memset(&recv_msg, 0, sizeof(recv_msg));
    int ret = server_recv(sock_fd, &recv_msg);
    if(ret < 0){
        return -1;
    }
    printf("获取帐号密码\n");
    // printf("account=%s\n", recv_msg.account);
    // printf("password=%s\n", recv_msg.password);
    // printf("data=%s\n", recv_msg.data);

    //验证账号密码
    //成功返回1，失败返回-1
    //.....

    //处理验证结果
    if(0){    //验证失败
        printf("failed\n");
        return -1;
    }
    //验证成功
    printf("验证成功回复client\n");
    struct send_message_t send_msg;
    memset(&send_msg, 0,sizeof(send_msg));
    send_msg.flag = 0;//验证账号回复标志
    send_msg.verify = 0;//0验证成功，1已登录，-1其他失败
    memcpy(send_msg.account, "1454865804", 10);
    memcpy(send_msg.data, "hello yds!", 11);
    // printf("帐号：%s\n",send_msg.account);
    // printf("内容：%s\n",send_msg.data);
    ret = server_send(sock_fd, &send_msg);
    if(ret < 0){
        return -1;
    }

    //添加至在线列表
    printf("添加至在线列表\n");
    add_member(sock_fd, recv_msg.account);

    //通知其他人有新的客户端上线
    printf("通知其他客户端\n");
    memset(&send_msg, 0, sizeof(send_msg));
    memcpy(send_msg.account, recv_msg.account, 10);
    // printf("帐号：%s\n",send_msg.account);
    // printf("内容：%s\n",send_msg.data);
    send_to_all(ON_LINE, &send_msg);
    return 0;
}

/**
 * 功能：添加到在线列表
 * 参数sock_fd：客户端描述符
 * 参数account：客户端账号
 * 返回值：无
*/
void add_member(int sock_fd, char *account)
{
    struct online_list_t *member = (struct online_list_t *)malloc(sizeof(struct online_list_t));
    memset(member, 0, sizeof(struct online_list_t));
    member->sock_fd = sock_fd;
    memcpy(member->account, account, 10);
    struct online_list_t *temp = online_list.next;
    online_list.next = member;
    member->next = temp;
    online_list.number +=1;
    printf("添加完成，现在有%d个人在线\n", online_list.number);
}

/**
 * 功能：从在线列表删除
 * 参数sock_fd：客户端描述符
 * 参数account：客户端账号
 * 返回值：无
*/
void delete_member(int sock_fd, char *account)
{
    struct online_list_t *member1 = &online_list;
    struct online_list_t *member2 = member1->next;
    while(member2 != NULL){
        //跳过自己
        if(sock_fd == member2->sock_fd){ 
            //printf("找到%s\n",member2->account);
            memcpy(account, member2->account, 10);
            printf("删除帐号：%s\n",account);
            member1->next = member2->next;
            free(member2);
            member2 = NULL;
            break;
        }
        member1 = member2;
        member2 = member2->next;
    }
    online_list.number -=1;
    printf("删除完成，现在有%d个人在线\n", online_list.number);
}

/**
 * 功能：给所有客户端发消息
 * 参数msg：消息内容
 * 参数type_t: 类型
 * 返回值：-1失败，0成功
*/
int send_to_all(enum types type_t, struct send_message_t *msg)
{
    switch(type_t)
    {
        case ON_LINE:   client_on_or_out_line(msg->account, 1);
                        break;
        case OUT_LINE:  client_on_or_out_line(msg->account, 0);
                        break;
        case SEND_MSG:  send_to_allmember(msg);break;

        default:        break;
    }
}

/**
 * 功能：通知所有客户端有新的客户端上线或下线
 * 参数account：此客户端账号
 * 参数flag：1上线，0下线
 * 返回值：无
*/
void client_on_or_out_line(char *account, unsigned char flag)
{
    //通知所有客户端我上线了
    struct online_list_t *member = online_list.next;
    while(member != NULL){
        //跳过自己
        if(memcmp(member->account, account, 10) == 0){ 
            member = member->next;
            continue;
        }
        //逐个通知
        struct send_message_t send_msg;
        memset(&send_msg, 0, sizeof(send_msg));
        send_msg.flag = 1;//client上下线消息标志
        send_msg.verify = flag;//1表示上线/0下线
        memcpy(send_msg.account, account, 10);//上线账号
        memcpy(send_msg.data, "online!", 8);
        server_send(member->sock_fd, &send_msg);
        member = member->next;
    }
    printf("通知所有客户端完成\n");
}

/**
 * 功能：群发消息
 * 参数msg：消息
 * 返回值：无
*/
void send_to_allmember(struct send_message_t *msg)
{
    struct online_list_t *member = online_list.next;
    while(member != NULL){
        //跳过自己
        if(memcmp(member->account, msg->account, 10) == 0){ 
            member = member->next;
            continue;
        }
        //逐个发送
        server_send(member->sock_fd, msg);
        member = member->next;
    }
    printf("群发消息完成\n");
}

/**
 * 功能：查找账号对应的私聊对象描述符
 * 参数：账号
 * 返回值：成功返回描述符，失败返回-1
*/
int get_member_sockfd(char *account)
{
    struct online_list_t *member = online_list.next;
    while(member != NULL){
        //找到账号对应的socket描述符，返回
        if(memcmp(member->account, account, 10) == 0){ 
            return member->sock_fd;
        }
        member = member->next;
    }
    //遍历了没找到
    printf("查找私聊对象完成\n");
    return -1;
}

/**
 * 功能：打印所有在线客户端
 * 参数：无
 * 返回值：无
*/
void show_all_members(void)
{
    struct online_list_t *member = online_list.next;
    printf("所有在线成员信息如下：\n");
    if(member == NULL){
        printf("没有人在线\n");
    }
    while(member != NULL){
        printf("sock_fd=%d,account=%s\n", member->sock_fd, member->account);
        member = member->next;
    }
}

/**
 * 功能：给刚上线的客户端同步所有在线成员
 * 参数：无
 * 返回值：无
*/
void sync_online_members_to_client(int sock_fd)
{
    struct online_list_t *member = online_list.next;
    if(member == NULL){
        return;
    }
    struct send_message_t msg;
    memset(&msg, 0, sizeof(msg));
    memcpy(&msg.data, "online", 8);
    while(member != NULL){
        if(member->sock_fd == sock_fd){
            //跳过自己
            member = member->next;
            continue;
        }
        msg.flag = 1;
        msg.verify = 1;
        memcpy(msg.account, member->account, 10);
        printf("同步：%s\n", msg.account);
        server_send(sock_fd, &msg);
        member = member->next;
    }
    printf("同步在线人数完成\n");
}