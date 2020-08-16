#ifndef CHAT_H
#define CHAT_H

#include <QWidget>
#include <QPushButton>
#include <QTextBrowser>
#include <QTextEdit>

class Chat : public QWidget
{
    Q_OBJECT
public:
    explicit Chat(QWidget *parent = 0);
    ~Chat();
    void appendMsg(QString msg);

private:
    QTextBrowser *content;
    QTextEdit *sendMsg;
    QPushButton *sendBtn;
    QPushButton *clearBtn;

signals:
    void signalSendMsg(QString *msg);

public slots:
    void slotClearContent();
    void slotSendBtn();
};

#endif // CHAT_H
