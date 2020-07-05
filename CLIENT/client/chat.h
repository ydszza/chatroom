#ifndef CHAT_H
#define CHAT_H

#include <QWidget>
#include <QPushButton>
#include <QTextBrowser>
#include <QTextEdit>


class chat : public QWidget
{
    Q_OBJECT
public:
    explicit chat(QString *account_t,QWidget *parent = 0);
    ~chat();
    void show_new_msg(QString);
    void set_account(QString account_t);

private:
    QPushButton *send_msg;
    QTextEdit *msg;
    QPushButton *clear_history;
    QTextBrowser *chat_content;
    QString account;//聊天对象账号

signals:
    void signal_send_msg(QString *msg);

public slots:
    void slot_clear_history();
    void slot_tell_home_send_msg();
};

#endif // CHAT_H
