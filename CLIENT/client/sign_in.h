#ifndef SIGN_IN_H
#define SIGN_IN_H

#include <QWidget>
#include <QTcpSocket>
#include "home.h"
#include "msg_t.h"
#include "mgs_manage.h"

namespace Ui {
class sign_in;
}

class sign_in : public QWidget
{
    Q_OBJECT

public:
    explicit sign_in(QWidget *parent = 0);
    ~sign_in();

private:
    Ui::sign_in *ui;
    home *Home;
    //QTcpSocket *socket;
    msg_manage *thread;
    QString account;
    QString password;

signals:
    void signal_sign_in(QString account, QString password);
//    void signal_sign_in_verify(msg_t *msg);
//    void signal_recv_msg(msg_t *msg);
//    void signal_add_or_out_newclient(msg_t *msg);

public slots:
    void slot_sign_in();
//    void slot_connected();
//    void slot_disconnected();
//    void slot_read_data();
//    void slot_send_data(QString *msg);
    void slot_sign_in_verify(msg_t *msg);
};

#endif // SIGN_IN_H
