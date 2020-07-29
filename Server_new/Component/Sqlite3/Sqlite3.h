#ifndef __SQLITE3_H
#define __SQLITE3_H

#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>
#include <string.h>

typedef struct user_info_t {
    char account[20];
    char password[20];
    char nick_name[20];
}user_info;

enum mode{
    PASSWORD, 
    NICK_NAME
};


static int callback(void *data, int argc, char **argv, char **azColName);
user_info* sqlite3_find(char *account);
int sqlite3_insert(user_info* info);
int sqlite3_update(user_info* info, enum mode mod);
int sqlite3_delete(user_info* info);


#endif // !__SQLITE3_H