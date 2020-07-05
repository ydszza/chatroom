#ifndef HOME_H
#define HOME_H

#include <QWidget>
#include <QPushButton>
#include "chat.h"
#include "msg_t.h"
#include "list.h"

class home : public QWidget
{
    Q_OBJECT
public:
    explicit home(QWidget *parent = 0);
    ~home(){}
private:
    chat *Chat;
    member_list *list;

signals:
    void signal_home_send_msg(QString *);

public slots:
//    void slot_enter_chat();
    void slot_add_or_out_newclient(msg_t *);
    void slot_get_new_msg(msg_t *);
    void slot_send_msg(QString *);
};


#endif // HOME_H
