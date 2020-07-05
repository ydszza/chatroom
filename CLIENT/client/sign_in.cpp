#include "sign_in.h"
#include "ui_sign_in.h"
#include <QDebug>
#include <QtNetwork>
#include <QMessageBox>
#include <cstring>

/*构造函数，布局页面*/
sign_in::sign_in(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::sign_in)
{
    this->setFixedSize(400, 300);
    ui->setupUi(this);
    connect(ui->pushButton_sign_in, SIGNAL(clicked()), this, SLOT(slot_sign_in()));
}

/*析构函数*/
sign_in::~sign_in()
{
    delete ui;
}

/*登录槽函数，进行账号密码有效判断并连接服务器*/
void sign_in::slot_sign_in()
{
    //获取账号密码
    account = ui->lineEdit_account->text();
    password = ui->lineEdit_password->text();
    //判断是否为空，为空直接返回
    if(!account.isEmpty() && !password.isEmpty())
    {
        //创建连接发送并验证
        //qDebug()<<sock_fd;
        qDebug()<<"点击登陆显示："<<account+" "<<password;
    }
    else
    {
        QMessageBox::information(this,tr("错误"),tr("账号或密码不能为空"));
        return;
    }
    this->ui->pushButton_sign_in->setEnabled(false);
    thread = new msg_manage;
    //thread->run();
    connect(this, SIGNAL(signal_sign_in(QString,QString)), thread, SLOT(slot_sign_in(QString, QString)));
    connect(thread, SIGNAL(signal_sign_in_verify(msg_t *)),this, SLOT(slot_sign_in_verify(msg_t *)));
    emit signal_sign_in(account, password);
}

/*验证登录状态槽函数*/
void sign_in::slot_sign_in_verify(msg_t *msg)
{
    qDebug()<<"get verify msg!";
    if(msg->verify == 0)
    {
        this->ui->pushButton_sign_in->setEnabled(false);
        //登录成功
        Home = new home;
        connect(thread, SIGNAL(signal_add_or_out_newclient(msg_t *)),Home, SLOT(slot_add_or_out_newclient(msg_t *)));
        connect(thread, SIGNAL(signal_get_new_msg(msg_t*)),Home, SLOT(slot_get_new_msg(msg_t*)));
        connect(Home, SIGNAL(signal_home_send_msg(QString*)),thread, SLOT(slot_send_data(QString*)));
        qDebug()<<"验证账号成功";
        this->hide();
        Home->show();
    }
    else if(msg->verify == 1)
    {
        //该账号已登录
        this->ui->pushButton_sign_in->setEnabled(true);
    }
    else
    {
        //账号或密码错误
        QMessageBox::information(this,tr("错误"),tr("账号或密码错误"));
        this->ui->pushButton_sign_in->setEnabled(true);
    }
    //验证完成，释放消息s
    delete msg;
    msg = NULL;
}
