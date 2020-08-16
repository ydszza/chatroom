#include "listmanage.h"
#include <QDebug>


treeNode::treeNode(QString acc, QString name, int size, QWidget *p):
    account(acc), nickName(name), next(nullptr){
    item = new QPushButton(nickName, p);
    item->setGeometry(0, size*50, 290, 50);
    connect(item, SIGNAL(clicked()), this, SLOT(slotShowChat()));
    item->show();

    chat = new Chat;
    connect(chat, SIGNAL(signalSendMsg(QString *)), this, SLOT(slotAddTouser(QString *)));
}
void treeNode::slotShowChat()
{
    this->chat->show();
}
void treeNode::slotAddTouser(QString *msg)
{
    QString tmp = account + QString(20-account.size(), '\0');
    msg->insert(0, tmp);
    qDebug() << *msg;
    emit signalSendtolist(msg);
}


ListManage::ListManage(Home *p) : parent(p)
{
    list = new QScrollArea(parent);
    list->setGeometry(0, 70, 290, 530);
    scrollWidget = new QWidget();
    list->setWidget(scrollWidget);
    list->setStyleSheet("QScrollBar {height:0px;width:0px}");

    size = 0;

    head = new treeNode("0000000000", "all", size, scrollWidget);
    tail = head;
    connect(head, SIGNAL(signalSendtolist(QString *)), parent, SLOT(slotSendMsg(QString *)));

    scrollWidget->setGeometry(0,0, 290, size*50+50);
}

ListManage::~ListManage()
{
    qDebug()<<2;
    delete scrollWidget;
    //scrollWidget = nullptr;
    delete list;
    //list = nullptr;
    while(head != tail) {
        treeNode *to_del = head;
        head = head->next;
        delete to_del;
    }
    qDebug()<<1;
    delete head;
}

void ListManage::add(ClientInfo *info)
{
    if(!info) return;
    qDebug() << "add" << info->account << info->nickname;
    size++;
    treeNode* newNode = new treeNode(info->account, info->nickname, size, scrollWidget);
    connect(newNode, SIGNAL(signalSendtolist(QString *)), parent, SLOT(slotSendMsg(QString *)));
    scrollWidget->setGeometry(0,0, 290, size*50+50);
    tail->next = newNode;
    tail = tail->next;
}

void ListManage::sub(ClientInfo *info)
{
    if(!info) return;
    qDebug() << "sub" << info->account << info->nickname;
    treeNode *node = head, *to_del = nullptr;
    while(node->next) {
        if(node->next->account == info->account) {
            to_del = node->next;
            node->next = node->next->next;
            size--;
            delete to_del;
            to_del = nullptr;

            //a empty btn

            return;
        }
        node = node->next;
    }
    qDebug()<<"no this member";
}

void ListManage::addNewMsg(Message* msg)
{
    treeNode *node = head;
    while(node) {
        if(node->account == msg->to) {
            if(msg->from == parent->getAccount())
                node->chat->appendMsg("me: \n    " + msg->data);
            else
                node->chat->appendMsg(msg->from + "\n    " + msg->data);
            delete msg;
            return;
        }
        node = node->next;
    }
    qDebug() << "no this member";
}

