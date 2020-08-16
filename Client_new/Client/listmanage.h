#ifndef LISTMANAGE_H
#define LISTMANAGE_H

#include <QWidget>
#include <QPushButton>
#include<QScrollArea>
#include "msgmanage.h"
#include "chat.h"
#include "home.h"

class treeNode : public QObject
{
    Q_OBJECT
public:
    QString account;
    QString nickName;
    QPushButton *item;
    Chat *chat;
    treeNode *next;
    treeNode(QString acc, QString name, int size, QWidget *p = 0);
    ~treeNode(){ delete chat; delete item; }

signals:
    void signalSendtolist(QString *);

public slots:
        void slotShowChat();
        void slotAddTouser(QString *);
};

class Home;
class ListManage : QObject
{
    Q_OBJECT
public:
    ListManage(Home* p = 0);
    ~ListManage();
    void add(ClientInfo *);
    void sub(ClientInfo *);
    void addNewMsg(Message* msg);

private:
    treeNode *head;
    treeNode *tail;
    int size;

    QScrollArea *list;
    Home *parent;
    QWidget *scrollWidget;
};


#endif // LISTMANAGE_H
