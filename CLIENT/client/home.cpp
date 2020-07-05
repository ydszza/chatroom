#include "home.h"
#include <QDebug>

home::home(QWidget *parent) : QWidget(parent)
{
    Chat = NULL;
    QString title_text = "主页";
    this->setWindowTitle(title_text);
    this->setFixedSize(290, 600);
    //初始化在线列表
    QString name_t("群聊");
    list = new member_list(&name_t);
    list->member->setParent(this);
    //connect(list->member, SIGNAL(clicked()), this, SLOT(slot_enter_chat()));
    //绑定打开聊天界面按钮
    connect(list->member, &QPushButton::clicked, [=](){
        list->is_open = true;
        //list->chat_page->set_account("0000000000");
        list->chat_page->show();
    });
    //绑定聊天界面发送消息按钮
    connect(list->chat_page, SIGNAL(signal_send_msg(QString *)), this, SLOT(slot_send_msg(QString *)));
}

/*get client in or out*/
void home::slot_add_or_out_newclient(msg_t *msg)
{
    if(msg->verify == 1)
    {
        //上线
        qDebug()<<"get add client msg!";
        member_list *temp = list->next;
        list->next = new member_list(&msg->account);
        list->next->member->setParent(this);
        list->next->member->show();
        //connect(list->next->member, SIGNAL(clicked()), this, SLOT(slot_enter_chat()));
        connect(list->next->member, &QPushButton::clicked, [=](){
            qDebug()<<1;
            list->next->is_open = true;
            qDebug()<<1;
            //list->next->chat_page->set_account(msg->account.toLatin1());
            qDebug()<<1;
            list->next->chat_page->show();
            qDebug()<<1;
        });
        //绑定聊天界面发送消息按钮
        connect(list->next->chat_page, SIGNAL(signal_send_msg(QString *)), this, SLOT(slot_send_msg(QString *)));
        list->next->next = temp;

    }
    else if(msg->verify == 0)
    {
        //下线
        qDebug()<<"get out client msg!";
        member_list *t1 = list;
        member_list *t2 = t1->next;
        int cnt = 1;
        qDebug()<<"begin to search";
        //search the account and delete
        while(t2 != NULL)
        {
            if(msg->account == t2->member->text())
            {
                qDebug()<<"delate account: "<<t2->member->text();
                t1->next = t2->next;
                delete t2;
                t2 = NULL;
                break;
            }
            t1 = t2;
            t2 = t2->next;
        }
        //在显示界面删除
        t1 = list;
        while(t1->next != NULL)
        {
            t1->next->member->setGeometry(0, cnt*50, 290, 50);
            cnt++;
            t1 = t1->next;
        }
    }
    else
    {
        //其他忽略
    }

    //消息处理完成，释放消息
    delete msg;
    msg = NULL;

    //show all member
    qDebug()<<"show all member:"<<list->count;
    member_list *temp = list->next;
    if(temp == NULL)
    {
        qDebug()<<"only you online!";
    }
    while(temp != NULL)
    {
        qDebug()<<temp->member->text();
        temp = temp->next;
    }
}

/*接受到消息槽函数*/
void home::slot_get_new_msg(msg_t *msg)
{
    //把消息填入chat页面内
    member_list *temp = list->next;
    if(temp == NULL)
    {
        qDebug()<<"only you online!";
    }
    //群聊消息
    if(msg->verify == 0)
    {
        list->chat_page->show_new_msg(msg->account);
        list->chat_page->show_new_msg(msg->data);
        return;
    }
    //私聊消息
    while(temp != NULL)
    {
        if(msg->account == temp->member->text())
        {
            temp->chat_page->show_new_msg(msg->account);
            temp->chat_page->show_new_msg(msg->data);
            return ;
        }
        temp = temp->next;
    }
    //释放消息
    delete msg;
    msg = NULL;
}

void home::slot_send_msg(QString *msg)
{
    //告诉消息管理器我要发消息
    qDebug()<<"home tell msg manage!";
    emit signal_home_send_msg(msg);
}

