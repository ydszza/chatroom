#include "mgs_manage.h"

msg_manage::msg_manage(QObject *parent):QThread(parent)
{
    socket = new QTcpSocket;
    socket->abort();
    connect(socket, SIGNAL(connected()), this, SLOT(slot_connected()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(slot_disconnected()));
    connect(socket, SIGNAL(readyRead()), this, SLOT(slot_read_data()));
}
/*析构函数*/
msg_manage::~msg_manage()
{
    delete socket;
}

/*开始登陆*/
void msg_manage::slot_sign_in(QString account_t, QString password_t)
{
    //连接服务器
    account = account_t;
    password = password_t;
    qDebug()<<"连接服务器前："<<account<<password;
    socket->connectToHost(SERVER_IP, PORT);
}

/*连接成功槽函数*/
void msg_manage::slot_connected()
{
    QString temp = account+password;
    temp.append("00011");
    //发送账号密码
    slot_send_data(&temp);
}

/*断开连接*/
void msg_manage::slot_disconnected()
{
    //断开连接
}

/*读取消息*/
void msg_manage::slot_read_data()
{
    //读取消息
    char recv_buf[1024] = {0};
    socket->read(recv_buf, 16);
    //解析接受的数据
    msg_t *msg = new msg_t;
    //获取消息类型
    msg->flag = recv_buf[0];
    //获取账号密码验证标志
    msg->verify = recv_buf[1];
    //获取消息来源账号
    char account_temp[11];
    account_temp[10] = '\0';
    memcpy(account_temp, recv_buf+2, 10);
    msg->account.append(account_temp);
    qDebug()<<"接受的消息：";
    qDebug()<<((int)msg->flag);
    qDebug()<<((int)msg->verify);
    qDebug()<<msg->account<<"长度："<<msg->account.length();
    //get length of data
    msg->length = recv_buf[12]*1000+recv_buf[13]*100+recv_buf[14]*10+recv_buf[15];
    //get data
    memset(recv_buf, 0, 1024);
    socket->read(recv_buf, msg->length);
    //获取消息内容
    msg->data.append(recv_buf);
    //打印接收到的消息查看
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
    case 2: emit signal_get_new_msg(msg);
            break;

    case 3: emit signal_creat_account(msg);
            break;
    //其他
    default: break;
    }
}

/*发送消息*/
void msg_manage::slot_send_data(QString *msg)
{
    //发送消息
    //this is a msg,not sign in verify
    if(account != msg->mid(0,10))
    {
        msg->insert(10,account);
    }
    qDebug()<<"发送消息前显示:"<<*msg;
    socket->write(msg->toLatin1());
}

void msg_manage::run()
{
    //线程函数
}

