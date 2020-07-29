#include "Socket.h"


/**
 * 初始化socket到listen
 * 参数：无
 * 返回值：成功：fd描述符，失败：-1
*/
int socket_init_to_listen(int* epoll_fd, ListNode** head)
{
    //初始化epoll
    *epoll_fd = epoll_create(1024);
    if(*epoll_fd < 0) {
        perror("epoll_create");
        goto ERR_EPOLL_CREATE;
    }
    printf("epoll_create...\n");

    //初始化监听套接字
    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(sock_fd < 0) {
        perror("socket");
        goto ERR_SOCK_FD;
    }
    //将socket设置为非阻塞模式,好处自行百度
	fcntl(sock_fd, F_SETFL, O_NONBLOCK); 
    printf("socket create %d\n", sock_fd);

    //将监听的fd挂到红黑树上
    struct epoll_event write_event;
    //设置读事件和边沿触发
    write_event.events = EPOLLIN | EPOLLET;
    //设置监听fd要传递的数据
    struct fd_info* info =(struct fd_info*)malloc(sizeof(struct fd_info));
    if(!info){
        goto ERR_MALLOC_INFO;
    }
    memset(info, 0, sizeof(struct fd_info));
    info->sock_fd = sock_fd;
    info->epoll_fd = *epoll_fd;
    info->buf = NULL;
    info->event = EPOLLIN;
    info->callback = socket_accept;
    info->head = head;
    write_event.data.ptr = (void *)info;
    //挂上树
    if(epoll_ctl(epoll_fd, EPOLL_CTL_ADD, sock_fd, &write_event) < 0){
        perror("epoll_ctl");
        goto ERR_PEOLL_CTL;
    }

    //绑定端口
    struct sockaddr_in server_addr;
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(IP_PORT);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if(bind(sock_fd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) < 0){
        perror("bind");
        goto ERR_BIND;
    }
    printf("bind...\n");
    
    //监听
    if(listen(sock_fd, 1024) < 0){
        perror("listen");
        goto ERR_LISTEN;
    }
    printf("listen...\n\n");
    return epoll_fd;

ERR_LISTEN:
ERR_BIND:
ERR_PEOLL_CTL:
    free(info);
ERR_MALLOC_INFO:
    close(sock_fd);
ERR_SOCK_FD:
     close(epoll_fd);
ERR_EPOLL_CREATE:
    return -1;
}

/**
 * 接收客户端连接
 * 参数：监听的socket套接字
 * 返回值：无
*/

void* socket_accept(void* arg)
{
    if(!arg){
        printf("param is NULL in %s\n", __func__);
        return NULL;
    }
    struct fd_info* server_info = (struct fd_info *)arg;
    struct sockaddr_in client_addr;
    socklen_t client_socket_len;
    int client_fd = accept(server_info->sock_fd, (struct sockaddr* )(&client_addr), &client_socket_len);
    if(client_fd < 0){
        return NULL;
    }
    //设置非阻塞
    if(fcntl(client_fd, F_SETFL, O_NONBLOCK) < 0){
        return NULL;
    }

    //挂到epoll树上
    struct epoll_event write_event;
    //读事件和边沿触发
    write_event.events = EPOLLIN | EPOLLET;
    //设置监听fd要传递的数据
    struct fd_info* info =(struct fd_info*)malloc(sizeof(struct fd_info));
    if(!info){
        return NULL;
    }
    memset(info, 0, sizeof(struct fd_info));
    info->sock_fd = client_fd;
    info->epoll_fd = server_info->epoll_fd;
    info->event = EPOLLIN;
    info->callback = socket_recv;
    info->buf = NULL;
    info->node = NULL;
    info->head = server_info->head;
    write_event.data.ptr = (void *)info;
    if(epoll_ctl(server_info->epoll_fd, EPOLL_CTL_ADD, client_fd, &write_event) < 0) {
        free(info);
        info = NULL;
        return NULL;
    };
    return NULL;
}

/**
 * 接受消息
 * 参数：对应套接字描述符
 * 返回值：无
*/
void* socket_recv(void* arg)
{
    if(!arg){
        return NULL;
    }
    struct fd_info* info = (struct fd_info *)arg;
    unsigned char flag;
    int len = recv(info->sock_fd, &flag, sizeof(flag), 0);
    if(len <= 0){
        perror("recv");
        //错误处理
        return NULL;
    }

    //分析信息头
    if(flag & RECV_MSG){//转发消息
        if(info->node == NULL){
            //跳过登录验证不允许，关闭该连接
            return NULL;
        }
        message* msg = (message*)malloc(sizeof(message));
        if(!msg){
            return NULL;
        }
        memset(msg, 0, sizeof(message));
        len = recv(info->sock_fd, msg->from_user, sizeof(message)-1, 0);//内存对齐？
        if(len < 0){
            perror("recv");
            return NULL;
        }
        msg->flag = RECV_MSG;
        info->buf = (char*)msg;
        info->len = sizeof(message);

    }else if(flag & SIGN_IN){//登录验证
        user_info register_user_info = {0};
        len = recv(info->sock_fd, (char *)&register_user_info, sizeof(user_info), 0);
        if(len <= 0){
            perror("recv");
            //错误处理
            return NULL;
        }

        info->buf = (char *)malloc(sizeof(char));
        if(info->buf == NULL){
            return NULL;
        }
        if(register_user(&register_user) != 0){
            info->buf = 0;
        }else{
            info->buf = (SIGN_IN);
            //添加到在线列表
            add_node_to_list(info->head, info->sock_fd, register_user_info.account, register_user_info.nick_name);
            //同步所有在线成员
            traversing_list_dosomething(info->head, )
        }

    }else if(flag & CHANGE_USERINFO){//修改个人信息
        user_info register_user_info = {0};
        len = recv(info->sock_fd, (char *)&register_user_info, sizeof(user_info), 0);
        if(len <= 0){
            perror("recv");
            //错误处理
            return NULL;
        }
        info->buf = (char *)malloc(sizeof(char));
        if(info->buf == NULL){
            return NULL;
        }
        if(updata_user_info(&register_user) != 0){
            info->buf = 0;
        }else{
            info->buf = (CHANGE_USERINFO);
        }

    }else if(flag & REGISTER){//注册帐号
        user_info register_user_info = {0};
        len = recv(info->sock_fd, (char *)&register_user_info, sizeof(user_info), 0);
        if(len <= 0){
            perror("recv");
            //错误处理
            return NULL;
        }
        info->buf = (char *)malloc(sizeof(char));
        if(info->buf == NULL){
            return NULL;
        }
        if(register_user(&register_user) != 0){
            info->buf = 0;
        }else{
            info->buf = (REGISTER);
        }
    }else{
        //其他情况无视
    }

    //从树上摘下重新设置为监听写事件
    epoll_ctl(info->epoll_fd, EPOLL_CTL_DEL, info->sock_fd, NULL);
    info->event = EPOLLOUT;
    info->callback = socket_send;
    struct epoll_event write_event;
    write_event.events = EPOLLOUT | EPOLLET;
    write_event.data.ptr = info;
    epoll_ctl(info->epoll_fd, EPOLL_CTL_ADD, info->sock_fd, &write_event);
    return NULL;
}

/**
 * 发送消息
 * 参数：发送消息结构体
 * 返回值：无
*/
void* socket_send(void* arg)
{
    if(!arg){
        return NULL;
    }
    struct fd_info* info = (struct fd_info *)arg;
    //私聊群聊转发
    if(info->buf[0] & RECV_MSG){
        message* msg = (message*)info->buf;
        if(strcmp(msg->to_user, "0000000000") == 0){
            //群聊转发
            //遍历在线列表并发送
        }else{
            //私聊转发
            //查找链表并发送
        }
    }else{
        //注册/个人信息修改/
        //登录回复
        //完成后修改监听为读事件
        if(send(info->sock_fd, info->buf, 1, 0) < 1){
            perror("send");
            //此fd出错从树上摘下并关闭.....
            return NULL;
        }
    }
    //发送完释放发送消息内存
    free(info->buf);
    info->buf = NULL;

    //从红黑树摘下并设置读事件
    epoll_ctl(info->epoll_fd, EPOLL_CTL_DEL, info->sock_fd, NULL);
    info->len = 0;
    info->event = EPOLLIN;
    info->callback = socket_recv;
    struct epoll_event write_event;
    write_event.events = EPOLLIN | EPOLLET;
    write_event.data.ptr = info;
    epoll_ctl(info->epoll_fd, EPOLL_CTL_ADD, info->sock_fd, &write_event);
    return NULL;
}



#if 0
/*test code*/

int main(void)
{
    int sock_fd = socket_init_to_listen();
    sleep(3);
    close(sock_fd);
    return 0;
}

#endif