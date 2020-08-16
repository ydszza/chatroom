#ifndef SIGNIN_H
#define SIGNIN_H

#include <QWidget>
#include "home.h"
#include "msgmanage.h"

#define SIGN_IN                 (1<<1)
#define CREATE                  (1<<0)

namespace Ui {
class SignIn;
}

class SignIn : public QWidget
{
    Q_OBJECT

public:
    explicit SignIn(QWidget *parent = 0);
    ~SignIn();

private:
    Ui::SignIn *ui;
    Home *home;
    MsgManage *msgManage;
    QString account;
    QString password;
    QString nickname;
    bool isCreate;

signals:
    void signalSignin(QString, QString);
    void signalCreate(QString account, QString password, QString nickname);

public slots:
    void slotBtnSigninClick();
    void slotSigninResult(QString);
    void slotBtnCreateClick();
    void slotCreateResult(char flag);
};

#endif // SIGNIN_H
