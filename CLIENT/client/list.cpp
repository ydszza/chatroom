#include "list.h"

int member_list::count = 0;

member_list::member_list(QString *account)
{
    member = new QPushButton(*account);
    member->setGeometry(0, count*50, 290, 50);
    next = NULL;
    is_open = false;
    QString temp = "群聊";
    if(temp == *account)
    {
        temp = "0000000000";
        chat_page = new chat(&temp);
    }
    else
    {
        chat_page = new chat(account);
    }
    count++;
}

member_list::~member_list()
{
    delete member;
    delete chat_page;
    count--;
}

