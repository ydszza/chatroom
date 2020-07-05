#include "server.h"

/**
 * 功能：创建socket/绑定/完成监听
 * 参数server_ip：ip地址
 * 参数port：端口
 * 返回值：-1失败，非负成功
*/
int server_init(char *server_ip, int server_port)
{
    //创建套接字
    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(sock_fd < 0){
		perror("socket create error");
		return -1;
	}
	printf("socket create success\n");

    //绑定套接字 即端口号 
	struct sockaddr_in server_addr;
	memset(&server_addr,0,sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(server_port);
	server_addr.sin_addr.s_addr =  inet_addr(server_ip);
	int ret = bind(sock_fd,(struct sockaddr *)&server_addr,sizeof(server_addr));
	if(ret < 0 ){
		perror("bind error");
		return -1;
	}
	printf("bind success\n");

	//监听 
	ret = listen(sock_fd,1000);
	if(ret < 0)
	{
		perror("listen error");
		return -1;
	}
    printf("listen ..........................\n");
    return sock_fd;
}

/**
 * 功能：接受客户端连接
 * 参数sock_fd：套接字描述符
 * 返回值：成功返回客户端描述符，失败返回-1
 */
int server_accept(int sock_fd)
{
	//等待客户端连接
	struct sockaddr_in client_addr;
	memset(&client_addr,0,sizeof(client_addr));
	int addr_len = sizeof(client_addr);
	int client_sock_fd = accept(sock_fd,(struct sockaddr *)&client_addr,(socklen_t*)&addr_len);
	if(client_sock_fd < 0)
	{
		perror("accept error");
		return -1;
	}
	printf("accept success,sock_fd = %d\n",client_sock_fd);
	return client_sock_fd;
}

/**
 * 功能：接收客户端消息
 * 参数sock_fd：客户端套接字描述符
 * 参数send_buf：要发送的消息，大小为1024
 * 返回值：失败返回-1，成功返回发送的字节数
 */
int server_send(int sock_fd, struct send_message_t *send_buf_t)
{
	int len = strlen(send_buf_t->data);
	send_buf_t->length[0] = len/1000%10;
	send_buf_t->length[1] = len/100%10;
	send_buf_t->length[2] = len/10%10;
	send_buf_t->length[3] = len%10;
	//发送前打印消息，便于查看
	printf("\n发送消息内容：\n");
	printf("flag=%d\n",send_buf_t->flag);
	printf("verify=%d\n",send_buf_t->verify);
	printf("account=%s\n", send_buf_t->account);
	printf("data length=%d%d%d%d\n", send_buf_t->length[0], 
								send_buf_t->length[1], send_buf_t->length[2],send_buf_t->length[3]);
	printf("data=%s\n\n", send_buf_t->data);
	char *send_buf = (char *)send_buf_t;
	/*发送消息*/
	int ret = send(sock_fd, send_buf, 16+len, 0);
	if(ret < 0){
		perror("send error");
		return -1;
	}
	return ret;
}

/**
 * 功能：发送消息
 * 参数sock_fd：客户端的套接字描述符
 * 参数recv_buf：接受消息的字符串，大小1024
 * 返回值：失败返回-1，成功返回接收的消息大小
 */
int server_recv(int sock_fd, struct recv_message_t*recv_buf_t)
{
	char *recv_buf = (char *)recv_buf_t;
	int ret = recv(sock_fd, recv_buf, 24, 0);
	if(ret <= 0){
		(ret == 0) ?  : perror("recv error");
		return -1;
	}
	int len = (recv_buf[20]-'0')*1000 + (recv_buf[21]-'0')*100 + (recv_buf[22]-'0')*10 + (recv_buf[23]-'0');
	//printf("data长度：%d\n", len);
	ret = recv(sock_fd, recv_buf+24, len, 0);
	if(ret < 0){
		perror("recv error");
		return -1;
	}
	//打印接收结果便于查看
	printf("\n接收消息内容：\n");
	char buffer[11] = {0};
	memcpy(buffer, recv_buf_t->account, 10);
	printf("account=%s\n", buffer);
	memset(buffer, 0, 11);
	memcpy(buffer, recv_buf_t->password, 10);
	printf("password=%s\n", buffer);
	memset(buffer, 0, 11);
	memcpy(buffer, recv_buf_t->length, 4);
	printf("data length=%s\n", buffer);
	printf("data=%s\n\n", recv_buf_t->data);
	return ret;
}
/**
 * 功能：断开socket连接
 * 参数sock_fd：要断开的连接
 * 返回值：失败返回-1，成功返回0
*/
int server_close(int sock_fd)
{
    int ret = close(sock_fd);
    if (ret < 0){
        perror("close error");
        return -1;
    }
    return 0;
}