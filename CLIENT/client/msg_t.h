#ifndef MSG_T_H
#define MSG_T_H

#include <QString>


class msg_t
{
public:
    msg_t();
    char flag;
    char verify;
    int length;
    QString account;
    QString data;
};

#endif // MSG_T_H
