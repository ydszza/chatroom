#include "msgmanage.h"
#include <QMessageBox>

MsgManage::MsgManage(QObject *parent):QThread(parent)
{
    socket = new QTcpSocket;
    socket->abort();
    connect(socket, SIGNAL(connected()), this, SLOT(slotConnected()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(slotDisconnected()));
    connect(socket, SIGNAL(readyRead()), this, SLOT(slotRecvData()));
}

MsgManage::~MsgManage()
{
    delete socket;
}

void MsgManage::slotSignin(QString account, QString password)
{
    this->account = account;
    this->password = password;

    socket->connectToHost(SERVER_ADDR, SERVER_PORT);
}

void MsgManage::slotCreate(QString account, QString password, QString nickname)
{
    this->account = account;
    this->password = password;
    this->nickname = nickname;

    socket->connectToHost(SERVER_ADDR, SERVER_PORT);
}

void MsgManage::slotConnected()
{
    qDebug()<< "connect success";
    account += QString(20 - account.size(), '\0');
    password += QString(20- password.size(), '\0');
    if(nickname.isEmpty()){
        QString sendDate = (unsigned char)(1 << 1) + account + password;
        slotSendData(&sendDate);
        return;
    }
    nickname += QString(20-nickname.size(), '\0');
    QString sendData = (unsigned char)(1 << 0) + account + password + nickname;
    nickname.clear();
    slotSendData(&sendData);
}

void MsgManage::slotDisconnected()
{
    qDebug()<< "disconnect";
    socket->abort();
    //QMessageBox::information(this, tr("error"), tr("disconnected!"));
    //send signal to home,exit to signin page
    //...
}

void MsgManage::slotSendData(QString *data)
{
    if(!data) return;
    qDebug()<< *data;
    socket->write(data->toLatin1());
    qDebug() << "send data success";
}

void MsgManage::slotRecvData()
{
    char recvFlag;
    qint64 len = 0;
    len = socket->read(&recvFlag, 1);
    if(len < 0) {
        return;
    }
    qDebug() << recvFlag;
    switch(recvFlag)
    {
    case FLAG_RECVMSG: recvMsg(); break;
    case FLAG_CLIENT: recvClientInfo(); break;
    case FLAG_SIGNIN: recvSigninResult(); break;
    case FLAG_UPDATE: recvUpdateResult(); break;
    case FLAG_CREATE: recvCreateResult(); break;
    default: break;
    }
}

void MsgManage::recvMsg()
{
    char newMsg[1064];
    qint64 len = socket->read(newMsg, 1064);
    if(len < 0) return;
    //qDebug() << newMsg;
    Message *msg = new Message;
    msg->from = newMsg;
    msg->to = newMsg + 20;
    msg->data = newMsg + 40;
    emit signalGetNewMsg(msg);
}

void MsgManage::recvClientInfo()
{
    char client[41];
    qint64 len = socket->read(client, sizeof(client));
    if(len < 0) return;
    qDebug() << len << client+1 << client+21;
    ClientInfo *info = new ClientInfo;
    //maybe a bug
    info->flag = client[0];
    info->account = client+1;
    info->nickname = client+21;
    qDebug() << "recv: " << info->account << info->nickname;
    emit signalClientState(info);
}

void MsgManage::recvSigninResult()
{
    char res;
    char nick_name[20];
    QString ret;
    qint64 len = socket->read(&res, sizeof(res));
    //qDebug() << res;
    if(len < 0) {//recv falg error
        socket->abort();

    } else {
        if(res) {//recv nickname
            len = socket->read(nick_name, 20);
            if(len > 0) {
                ret += nick_name;
                emit signalSignResult(ret);
                return;
            }
            socket->abort();
        }
    }

    emit signalSignResult(ret);
}

void MsgManage::recvUpdateResult()
{
    char res;
    qint64 len = socket->read(&res, sizeof(res));
    if(len < 0) emit signalSignResult(0);
    else emit signalUpdateResult(res);
}

void MsgManage::recvCreateResult()
{
    char res;
    qint64 len = socket->read(&res, sizeof(res));
    socket->abort();
    if(len < 0) emit signalSignResult(0);
    else emit signalCreateResult(res);
}

void MsgManage::closeConnect()
{
    socket->abort();
}













