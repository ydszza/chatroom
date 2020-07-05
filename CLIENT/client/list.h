#ifndef LIST_H
#define LIST_H
#include <QPushButton>
#include "msg_t.h"
#include "chat.h"


class member_list
{
public:
    member_list(QString *account);
    ~member_list();
    QPushButton *member;
    member_list *next;
    bool is_open;
    chat *chat_page;
    static int count;
};


#endif // LIST_H
