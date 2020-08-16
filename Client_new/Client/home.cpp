#include "home.h"

Home::Home(QString acc, QString nickname, QWidget *parent)
    : QWidget(parent), account(acc), nickName(nickname)
{
    this->setWindowTitle("Home");
    this->setFixedSize(290, 600);

    icon = new QPushButton(this);
    icon->setGeometry(10, 10, 50, 50);

    labelNickName = new QLabel(this);
    labelNickName->setText(nickName);
    labelNickName->setGeometry(80, 20, 80, 10);
    labelSignNature = new QLabel(this);
    labelSignNature->setText("leave a message....");
    labelSignNature->setGeometry(80, 40, 180, 10);

    onlineMember = new ListManage(this);
}

Home::~Home()
{
    delete icon;
    delete onlineMember;
    delete labelNickName;
    delete labelSignNature;
}

void Home::slotClientInfo(ClientInfo *info)
{
    if(!info) return;

    if(info->flag) {
        //new client online
        qDebug() << "online client: " << info->account << info->nickname;
        onlineMember->add(info);
    } else {
        //a client leave
        qDebug() << "outline client: " << info->account << info->nickname;
        onlineMember->sub(info);
    }
    delete info;
}

void Home::slotGetNewMsg(Message *msg)
{
    qDebug() << msg->from;
    qDebug() << msg->to;
    qDebug() << msg->data;
    onlineMember->addNewMsg(msg);
}

void Home::slotUpDate(char flag)
{
    if(flag) {
        qDebug()<<"success to update info";
    } else {
        qDebug()<<"error to update info";
    }
}

void Home::slotSendMsg(QString *msg)
{
    char flag = 1 << 3;
    QString tmp = account + QString(20-account.size(), '\0');
    msg->insert(0, flag + tmp);
    qDebug() << *msg;
    emit signalHomeSendMsg(msg);
}

