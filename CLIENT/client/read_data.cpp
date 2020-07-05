#include "read_data.h"
#include <QDebug>

/*构造函数*/
read_data::read_data()
{
    socket = new QTcpSocket;
    socket->abort();
    connect(socket, SIGNAL(connected()), this, SLOT(slot_connected()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(slot_disconnected()));
    connect(socket, SIGNAL(readyRead()), this, SLOT(slot_read_data()));
}

/*析构函数*/
read_data::~read_data()
{
    delete socket;
}

/*开始登陆*/
void read_data::slot_sign_in(QString account_t, QString password_t)
{
    //连接服务器
    account = account_t;
    password = password_t;
    qDebug()<<account;
    qDebug()<<password;
    socket->connectToHost(SERVER_IP, PORT);
}

/*连接成功槽函数*/
void read_data::slot_connected()
{
    QString temp = account+password;
    //发送账号密码
    slot_send_data(&temp);
}

/*断开连接*/
void read_data::slot_disconnected()
{
    //断开连接
}

/*读取消息*/
void read_data::slot_read_data()
{
    //读取消息
    char recv_buf[1024] = {0};
    socket->read(recv_buf, 1024);
    //解析接受的数据
    msg_t *msg = new msg_t;
    //获取消息类型
    msg->flag = recv_buf[0];
    //获取账号密码验证标志
    msg->verify = recv_buf[1];
    //获取消息来源账号
    char account_temp[11];
    memcpy(account_temp, recv_buf+2, 10);
    msg->account.append(account_temp);
    //获取消息内容
    msg->data.append(recv_buf+12);
    //打印接收到的消息查看
    qDebug()<<"接受的消息："<<(msg->flag+48);
    qDebug()<<(msg->verify+48);
    qDebug()<<msg->account<<"长度："<<msg->account.length();
    qDebug()<<msg->data;
    //分发消息
    switch(msg->flag)
    {
    //登录验证消息
    case 0: emit signal_sign_in_verify(msg);//接受到登录验证消息，发出信号
            break;

    //上线或下线通知
    case 1: emit signal_add_or_out_newclient(msg);
            break;

    //新的消息
    case 2: break;

    //其他
    default: break;
    }
}

/*发送消息*/
void read_data::slot_send_data(QString *msg)
{
    //发送消息
    qDebug()<<"发送前打印:"<<*msg;
    socket->write(msg->toLatin1());
}

void read_data::run()
{
    //线程函数
}
