#ifndef __OP_SQLITE3_H
#define __OP_SQLITE3_H

#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>
#include <string.h>

char *my_sqlite3_find(char *account_t);
int my_sqlite3_insert(char *account_t, char *password_t);
void my_sqlite3_update(char *account_t, char *password_t);
static int callback(void *data, int argc, char **argv, char **azColName);

#endif // !__OP_SQLITE3_H