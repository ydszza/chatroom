#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <poll.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <iostream>
using namespace std;
#define backlog 10
 
 
class Server
{
	private:
		int port;   // 定义端口号
		int listenfd;	// 定义监听套接字
		int clientfd;	// 定义客户端套接字
		int i;		
		int maxfd;	// 定义套接字最大描述符
		struct sockaddr_in clientaddr;	// 定义客户端地址结构体
		struct sockaddr_in listenaddr;	// 定义监听服务端地址结构体
		struct pollfd readfds[backlog];	// 定义poll套接字结构体，可理解为是一个数组
		socklen_t clientaddrlen;	// 定义客户端地址长度
	public:
		Server(int port = 10000):port(port),listenfd(-1),clientfd(-1)
		// 初始化端口号，套接字描述符
		{
			
			listenaddr.sin_family = AF_INET;	// 初始化监听套接字协议族为AF_INET
			listenaddr.sin_port = htons(port);	// 初始化监听套接字端口号
			listenaddr.sin_addr.s_addr = htonl(INADDR_ANY);	// 初始化监听套接字地址
			listenfd = socket(AF_INET,SOCK_STREAM | SOCK_NONBLOCK,0); // 创建套接字，并设置为非阻塞模式
			int bind_val = bind(listenfd,(struct sockaddr*)&listenaddr,sizeof(listenaddr));	// 绑定套接字
			bind_result(bind_val);	// 验证绑定结果
			int listen_val = listen(listenfd,backlog);	// 监听套接字
			listen_rsult(listen_val);	// 验证监听结果
		}
		void poll_run()
		{
			init_readfds(); 	// 初始化可读套接字
			while(1)
			{
				maxfd = get_max();	// 获取最大套接字
				int poll_val = poll(readfds,maxfd+1,5); // 检测各个套接字信号变化
				for(i=0;i<backlog;i++)
				{
					if(readfds[i].fd==listenfd)
					{
						clientfd = accept(listenfd,(struct sockaddr*)&clientaddr,&clientaddrlen);
						// 检测客户端连接请求
						if(accept_result(clientfd)==-1)
						{
							continue;
						}else if(accept_result(clientfd)==0)
						{
							continue;
						}else
						{
							add_client(clientfd);
						}
					}else if(readfds[i].fd>0)
					{
						char data[1024];
						int recv_val = recv(readfds[i].fd,data,sizeof(data),0);
						// 接受客户端的数据请求
						int rv = recv_result(recv_val);
						if(rv==-1)
						{
						    continue;
						}else if(rv==0)
						{
						    remove_client(readfds[i].fd);
						    continue;
						}else
						{
						  cout << data << "\n" ; 
						}
					}else
					{
						cout << "";
					}
				}
			}
 
		}
		void add_client(int c)
		// 添加客户端到可读套接字
		{
			readfds[c].fd = c;
			readfds[c].events = POLLIN;
		}
		void remove_client(int c)
		// 移除客户端套接字
		{
			readfds[c].fd = -1;
		}
		int get_max()
		// 获取最大描述符
		{
			maxfd = -1;
			for(i=0;i<backlog;i++)
			{
				if(readfds[i].fd>maxfd)
					maxfd = readfds[i].fd;
			}
			return maxfd;
		}
		
		int recv_result(int recv_val)
		// 校验recv结果
		{
			if(recv_val==-1)
			{
				cout << "Recv error..." << "\n";
				sleep(3);
				return -1;
			}else if(recv_val==0)
			{
				
				cout << "Client connect close..." << "\n";
				return 0;
			}else
			{
				return 1;
			}
		}
		int accept_result(int accept_val)
		// 校验accept结果
		{
			if(accept_val==-1)
			{
				//cout << "Accept error..." << "\n";
				return -1;
			}else if(accept_val==0)
			{
				 return 0;
			}else
			{
				return 1;
			}
		}
		int poll_result(int poll_val)
		// 校验poll结果
		{
			if(poll_val==-1)
			{
				cout << "Poll error..." << "\n";
				return -1;
			}else if(poll_val==0)
			{
				return 0;
			}else
			{
				return 1;
			}
		}
		void init_readfds()
		// 初始化可读套接字
		{
			for(i=0;i<backlog;i++)
			{
				readfds[i].fd = -1;
				readfds[i].events = POLLIN;
			}
			readfds[listenfd].fd = listenfd;
			readfds[listenfd].events = POLLIN;
		}
		void bind_result(int bind_val)
		// 校验绑定结果
		{
			if(bind_val<0)
			{
				cout << "Bind error..." << "\n";
				exit(0);
			}else
			{
				cout << "Bind successful" << "\n";
			}
		}
		void listen_result(int listen_val)
		// 校验监听结果
		{
			if(listen_val<0)
			{
				cout << "Listen error..." << "\n";
				exit(0);
			}else
			{
				cout << "Listen successful"<< "\n";
			}
		}
		~Server()
		{
			cout << "close serve..." << "\n";
			for(i=0;i<backlog;i++)
			{
				close(readfds[i].fd);	// 退出程序时,关闭所有套接字
			}
		}
};
 
int test()
{
	Server *s = NULL;	// 定义Server指针
	try
	{
		s = new Server; // 为指针分配内存空间
		s->poll_run(); // 运行poll
	}catch(const char* e)
	{
		cout << e << "\n";
	}
	delete s;  // 删除指针
}
 
int main()
{
	test();
}