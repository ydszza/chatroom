#include "Socket.h"

/**
 * 初始化监听套接字
 * 成功：fd,失败：-1
*/
int server_init( int epoll_fd, ListNode** head)
{
    int server_fd;
    do{
        server_fd = socket(AF_INET, SOCK_STREAM, 0);
        if(server_fd < 0)
            break;

        fcntl(server_fd, F_SETFL, O_NONBLOCK);

        //注册监听事件
        struct event_arg *arg = event_arg_create(epoll_fd, server_fd, EPOLLIN, head, socket_accept, NULL, 0);
        if(epoll_add_fd(epoll_fd, arg) < 0)
            break;
        
        //绑定端口
        struct sockaddr_in server_addr;
        bzero(&server_addr, sizeof(server_addr));
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(IP_PORT);
        server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
        if(bind(server_fd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) < 0)
            break;
        
        //监听
        if(listen(server_fd, 1024) < 0)
            break;

        printf("\nstart to listen...\n\n");
        return server_fd;
    }while(0);

    close(server_fd);
    return -1;
}


/**
 * 接受客户端连接
*/

void* socket_accept(void* arg)
{
    if(!arg){
        return NULL;
    }
    printf("deal with accept\n");
    struct event_arg *ev = (struct event_arg *)arg;

    //accept要连接的客户端
    struct sockaddr_in client_addr;
    bzero(&client_addr, sizeof(client_addr));
    socklen_t client_addrlen = sizeof(struct sockaddr);
    int client_fd = accept(ev->fd, (struct sockaddr *)&client_addr, &client_addrlen);
    if(client_fd < 0) 
        return NULL;
    //设置非阻塞
    fcntl(client_fd, F_SETFL, O_NONBLOCK);

    //注册epoll监听读事件
    struct event_arg *new_arg = event_arg_create(ev->epoll_fd, client_fd, EPOLLIN, ev->arg, socket_recv, NULL, 0);
    if(!new_arg){
        close(client_fd);
        return NULL;
    }
    if(epoll_add_fd(ev->epoll_fd, new_arg) < 0){
        return NULL;
    }
    return NULL;
}

/*转发消息*/
static int message_forwards(struct event_arg *ev)
{
    if(!ev) return -1;
    //接收要转发的消息
    message* msg = (message*)malloc(sizeof(message));
    if(!msg) return -1;

    int len = recv(ev->fd, (char *)msg->from_user, sizeof(message), 0);
    if(len < 0) return -1;

    msg->flag = RECV_MSG;
    ev->buf = (char *)msg;
    ev->buf_len = sizeof(message);

    if(strcmp(msg->to_user, TO_ALL) == 0) {
        ev->is_chat = 1;//置1为群聊消息
        printf("群聊\n");
    } else {
        ev->is_chat = 2;//2为私聊消息
        printf("私聊\n");
    }
    
    return 0;
}

/*登录验证*/
static int sign_in_check(struct event_arg *ev)
{
    if(!ev) return -1;
    user_info* info = (user_info *)malloc(sizeof(user_info));
    if(!info) return -1;
    memset(info, 0, sizeof(user_info));

    int len = recv(ev->fd, (char*)info, sizeof(user_info), 0);
    if(len < 0) return -1;
    printf("recv: account = %s \t password = %s \t nick_name = %s\n", info->account, info->password, info->nick_name);
    
    //验证帐号密码
    if(sign_in(info) == 0) {//验证成功
        ev->buf = (char *)malloc(sizeof(char)*22);
        if(ev->buf == NULL) return -1;
        memset(ev->buf, 0, sizeof(char)*22);
        ev->buf[0] = SIGN_IN;
        ev->buf[1] = 1;
        strcpy((ev->buf)+2, info->nick_name);
        ev->buf_len = sizeof(char)*22;

        //添加到在线列表
        add_node_to_list(ev->arg, ev->fd, info->account, info->nick_name);
        print_list(*(ListNode**)(ev->arg));

        //告诉其他客户端
        char buffer[42] = {0};//告诉其他客户端有新的客户端上线
        char buf[42] = {0};//为新客户端获取在线列表
        buffer[0] = buf[0] = 1 << 4;
        buffer[1] = buf[1] = 1;
        strcpy(buffer+2, info->account);
        strcpy(buffer+22, info->nick_name);
        ListNode* node = *(ListNode**)ev->arg;
        while(node) {
            if(node->sock_fd == ev->fd) {
                node = node->next;
                continue;
            }
            send(node->sock_fd, buffer, sizeof(buffer), 0);
            //有错
            strcpy(buf+2, node->account);
            strcpy(buf+22, node->nick_name);
            send(ev->fd, buf, sizeof(buf), 0);
            node = node->next;
        }
        printf("同步完成\n");


    }else {
        //验证失败
        //设置失败消息
        ev->buf = (char *)malloc(sizeof(char)*2);
        if(ev->buf == NULL) return -1;
        ev->buf[0] = SIGN_IN;
        ev->buf[1] = 0;
        ev->buf_len = sizeof(char)*2;
    }

    return 0;
}

/*修改个人信息*/
static int updata_userinfo(struct event_arg *ev)
{
    if(!ev) return -1;

    user_info* info = (user_info *)malloc(sizeof(user_info));
    if(!info) return -1;
    memset(info, 0, sizeof(user_info));

    int len = recv(ev->fd, (char*)info, sizeof(user_info), 0);
    if(len < 0) return -1;

    //验证帐号密码
    if(updata_user_info(info) == 0) {
        //验证成功
        //设置验证回复消息
        ev->buf = (char *)malloc(sizeof(char));
        if(!ev->buf) return -1;

        ev->buf = (char *)malloc(sizeof(char));
        if(ev->buf == NULL) return -1;
        *(ev->buf) = UPDATE_USERINFO;
        ev->buf_len = sizeof(char);

    }else {
        //验证失败
        //设置失败消息
        ev->buf = (char *)malloc(sizeof(char));
        if(!ev->buf) return -1;
        ev->buf = 0;
        ev->buf_len = sizeof(char);
    }
    return 0;
}

/*帐号注册*/
static int register_account(struct event_arg *ev)
{
    if(!ev) return -1;

    user_info* info = (user_info *)malloc(sizeof(user_info));
    if(!info) return -1;
    memset(info, 0, sizeof(user_info));

    int len = recv(ev->fd, (char*)info, sizeof(user_info), 0);
    if(len < 0) return -1;

    //验证帐号密码
    if(register_user(info) == 0) {
        //验证成功
        //设置验证回复消息
        ev->buf = (char *)malloc(sizeof(char));
        if(!ev->buf) return -1;

        *(ev->buf) = REGISTER;
        ev->buf_len = sizeof(message);

    }else {
        //验证失败
        //设置失败消息
        ev->buf = (char *)malloc(sizeof(char));
        if(!ev->buf) return -1;
        *(ev->buf) = 0;
        ev->buf_len = sizeof(char);
    }
    return 0;
}

static void client_outline(int fd, ListNode** head)
{
    ListNode* node = *head;
    while(node) {
        if(node->sock_fd == fd) {
            break;
        }
        node = node->next;
    }
    //告诉其他客户端
    char buffer[42] = {0};//告诉其他客户端有客户端下线
    buffer[0] = 1 << 4;
    buffer[1] = 0;
    strcpy(buffer+2, node->account);
    strcpy(buffer+22, node->nick_name);
    printf(" outline : account = %s \t nickname = %s \n", buffer+2, buffer+22);
    ListNode* p_node = *head;
    while(p_node) {
        if(p_node->sock_fd == fd) {
            p_node = p_node->next;
            continue;
        }
        send(p_node->sock_fd, buffer, sizeof(buffer), 0);
        p_node = p_node->next;
    }
    del_list_node(head, node);
}

/**
 * 接收消息
*/
void* socket_recv(void* arg)
{
    if(!arg){
        return NULL;
    }
    struct event_arg *ev = (struct event_arg *)arg;
    //接收消息头判断消息类型
    unsigned char flag;
    int len = recv(ev->fd, &flag, sizeof(flag), 0);
    if(len <= 0){
        perror("recv");
        //客户端下线
        client_outline(ev->fd, (ListNode**)ev->arg);
        print_list(*(ListNode**)ev->arg);
        epoll_del_fd(ev->epoll_fd, ev->fd);
        free(ev);
        return NULL;
    }

    //判断消息类型
    if(flag & RECV_MSG){//转发消息
        //先验证是否登录
        //...
        printf("msg forwarded\n");
        message_forwards(ev);

    }else if(flag & SIGN_IN){//登录验证
        printf("sign_in_check\n");
        sign_in_check(ev);
        //检查成功需要通知其他在线客户端
        //...

    }else if(flag & UPDATE_USERINFO){//更新个人信息
        printf("update user info\n");
        updata_userinfo(ev);

    }else if(flag & REGISTER){//注册账号
        printf("register user\n");
        register_account(ev);

    }else{
        //忽略其他情况
        printf("error flag\n");
    }

    //消息处理结束，修改监听事件由读事件改为写事件
    //从epoll事件删除
    if(epoll_del_fd(ev->epoll_fd, ev->fd) < 0) {
        //错误处理
        //...
        return NULL;
    }
    //修改监听写
    ev->event = EPOLLOUT;
    //修改回调函数
    ev->callback = socket_send;
    //重新添加到epoll
    if(epoll_add_fd(ev->epoll_fd, ev) < 0){
        //错误处理
        //...
        return NULL;
    }
    printf("recv msg successful\n");
    return NULL;
}

/**
 * 发送消息函数
*/
void* socket_send(void* arg)
{
    if(!arg) return NULL;

    struct event_arg *ev = (struct event_arg *)arg;
    if(ev->is_chat == 1) {//群聊消息
        ListNode* node = *(ListNode** )ev->arg;
        while(node) {
            send(node->sock_fd, ev->buf, ev->buf_len, 0);
            node = node->next;
        }
        
    } else if(ev->is_chat == 2) {//私聊消息
        ListNode** head = (ListNode** )ev->arg;
        int fd = find_fd_in_list(*head, (ev->buf)+21);
        if(fd < 0) return NULL;
        send(fd, ev->buf, ev->buf_len, 0);

    } else {//验证消息
        int len = send(ev->fd, ev->buf, ev->buf_len, 0);
        if(len < 0) return NULL;
    }

    //判断是否为注册信息或登录失败信息，是的话注销监听
    if(*(ev->buf) == REGISTER || (ev->buf[0] == SIGN_IN && ev->buf[1] == 0)) {
        close(ev->fd);
        epoll_del_fd(ev->epoll_fd, ev->fd);
        free(ev->buf);
        free(ev);
        return NULL;
    }

    //释放消息
    free(ev->buf);
    ev->buf = NULL;
    ev->buf_len = 0;

    //修改为读监听事件
    epoll_del_fd(ev->epoll_fd, ev->fd);
    
    ev->event = EPOLLIN;
    ev->callback = socket_recv;
    epoll_add_fd(ev->epoll_fd, ev);

    printf("send msg successful\n");
    return NULL;
}