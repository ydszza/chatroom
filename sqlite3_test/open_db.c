#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>

int main(int argc, char *argv[])
{
    sqlite3 *db;
    char *zErr = NULL;
    int rc;
    rc = sqlite3_open("test.db", &db);
    if( rc ){
        fprintf(stderr, "sqlite3_open database: %s\n", sqlite3_errmsg(db));
        exit(0);
    }else{
        fprintf(stderr, "open database successfully\n");
    }
    sqlite3_close(db);
}