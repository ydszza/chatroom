#include "chat.h"
#include <QDebug>

chat::chat(QString *account_t,QWidget *parent) : QWidget(parent)
{
    this->account.append(account_t);
    this->setFixedSize(600, 450);
    this->setWindowTitle("聊天");
    //显示聊天框
    this->chat_content = new QTextBrowser(this);
    //this->chat_content->append("lll");
    //显示消息输入框
    this->msg = new QTextEdit(this);
    //发送消息按键
    this->send_msg = new QPushButton(this);
    this->send_msg->setText("发送");
    //清除聊天记录按键
    this->clear_history = new QPushButton(this);
    this->clear_history->setText("清除");
    //上述组件布局
    this->chat_content->setGeometry(0, 0, 600, 280);
    this->msg->setGeometry(0, 280, 600, 120);
    this->clear_history->setGeometry(this->width()-20-70-60, this->height()-10-24, 60, 24);
    this->send_msg->setGeometry(this->width()-20-60, this->height()-10-24, 60, 24);
    connect(clear_history, SIGNAL(clicked()), this, SLOT(slot_clear_history()));
    connect(send_msg, SIGNAL(clicked()), this, SLOT(slot_tell_home_send_msg()));
}
chat::~chat()
{
    //
}

void chat::show_new_msg(QString msg)
{
    chat_content->append(msg);
}

void chat::slot_clear_history()
{
    chat_content->clear();
}

void chat::slot_tell_home_send_msg()
{
    //发送
    qDebug()<<"点击发送消息按钮";
    //输入框为空不发送
    if(msg->toPlainText().isEmpty())
    {
        return;
    }
    QString send_msg;
    send_msg.append(account);
    //send_msg.append("0000000000");
    int len = msg->toPlainText().length();
    char length[5] = {0};
    length[0] = len/1000%10+'0';
    length[1] = len/100%10+'0';
    length[2] = len/10%10+'0';
    length[3] = len%10+'0';
    length[4] = '\0';
    send_msg.append(length);
    send_msg.append(msg->toPlainText());
    emit signal_send_msg(&send_msg);
    chat_content->append("me");
    chat_content->append(msg->toPlainText());
    msg->clear();
}

void chat::set_account(QString account_t)
{
    qDebug()<<1;
    account = account_t;
}

