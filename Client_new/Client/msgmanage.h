#ifndef MSGMANAGE_H
#define MSGMANAGE_H

#include <QObject>
#include <QTcpSocket>
#include <QThread>

#define SERVER_ADDR     "127.0.0.1"
#define SERVER_PORT     12345

#define FLAG_CREATE     (1 << 0)
#define FLAG_SIGNIN     (1 << 1)
#define FLAG_UPDATE     (1 << 2)
#define FLAG_RECVMSG    (1 << 3)
#define FLAG_CLIENT     (1 << 4)

struct ClientInfo {
    char flag;
    QString account;
    QString nickname;
};

struct Message {
    QString from;
    QString to;
    QString data;
};

class MsgManage : public QThread
{
    Q_OBJECT //using signal and slot
public:
    explicit MsgManage(QObject *parent = 0);
    ~MsgManage();
    void closeConnect();

private:
    QTcpSocket *socket;
    QString account;
    QString password;
    QString nickname;
    void recvMsg();
    void recvClientInfo();
    void recvSigninResult();
    void recvUpdateResult();
    void recvCreateResult();


signals:
    void signalSignResult(QString);
    void signalCreateResult(char);
    void signalUpdateResult(char);
    void signalClientState(ClientInfo*);
    void signalGetNewMsg(Message*);

public slots:
    void slotSignin(QString, QString);
    void slotCreate(QString, QString, QString);
    void slotConnected();
    void slotDisconnected();
    void slotRecvData();
    void slotSendData(QString*);
};

#endif // MSGMANAGE_H
