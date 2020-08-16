#ifndef HOME_H
#define HOME_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include "listmanage.h"
#include "msgmanage.h"

class ListManage;
class Home : public QWidget
{
    Q_OBJECT
public:
    explicit Home(QString account = "", QString nickname = "", QWidget *parent = 0);
    ~Home();
    QString getAccount(){ return account;}

private:
    QPushButton *icon;
    QLabel *labelNickName;
    QLabel *labelSignNature;
    QString account;
    QString nickName;
    ListManage *onlineMember;

signals:
    void signalHomeSendMsg(QString *);

public slots:
    void slotClientInfo(ClientInfo *);
    void slotGetNewMsg(Message *);
    void slotUpDate(char);
    void slotSendMsg(QString *);
};

#endif // HOME_H
