#ifndef READ_DATA_H
#define READ_DATA_H
#include <QThread>
#include <QTcpSocket>
#include "msg_t.h"

#define SERVER_IP   "192.168.124.5"
#define PORT        12345


class read_data : public QThread
{
public:
    read_data();
    ~read_data();
protected:
    virtual void run();
private:
    QTcpSocket *socket;
    QString account;
    QString password;

signals:
    void signal_sign_in_verify(msg_t *);
    void signal_add_or_out_newclient(msg_t *);

public slots:
    void slot_sign_in(QString account_t, QString password_t);
    void slot_connected();
    void slot_disconnected();
    void slot_read_data();
    void slot_send_data(QString *);
};

#endif // READ_DATA_H
