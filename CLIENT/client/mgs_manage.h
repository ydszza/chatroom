#ifndef msg_manage_H
#define msg_manage_H

#include <QObject>
#include <QThread>
#include <QTcpSocket>
#include "msg_t.h"

#define SERVER_IP   "192.168.124.2"
#define PORT        12345

class msg_manage : public QThread
{
    Q_OBJECT //使用信号与槽函数
public:
    explicit msg_manage(QObject *parent = 0);
    ~msg_manage();
protected:
    virtual void run();
private:
    QTcpSocket *socket;
    QString account;
    QString password;

signals:
    void signal_sign_in_verify(msg_t *);
    void signal_add_or_out_newclient(msg_t *);
    void signal_get_new_msg(msg_t *);

public slots:
    void slot_sign_in(QString account_t, QString password_t);
    void slot_connected();
    void slot_disconnected();
    void slot_read_data();
    void slot_send_data(QString *);
};

#endif // msg_manage_H
