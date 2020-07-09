#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>

static int callback(void *NotUsed, int argc, char **argv, char **azColName)
{
    int i;
    for (i = 0;i<argc;i++) {
        printf("%s = %s\n",azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}

int main(int argc, char *argv[])
{
    sqlite3 *db;
    char *zErr = NULL;
    int rc;
    char *sql;

    //open database
    rc = sqlite3_open("test.db", &db);
    if( rc ){
        fprintf(stderr, "open database error: %s\n", sqlite3_errmsg(db));
        exit(1);
    }else{
        fprintf(stderr, "open database successfully\n");
    }

    //create sql statement
    sql = "INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY) "  \
         "VALUES (1, 'Paul', 32, 'California', 20000.00 ); " \
         "INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY) "  \
         "VALUES (2, 'Allen', 25, 'Texas', 15000.00 ); "     \
         "INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY)" \
         "VALUES (3, 'Teddy', 23, 'Norway', 20000.00 );" \
         "INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY)" \
         "VALUES (4, 'Mark', 25, 'Rich-Mond ', 65000.00 );";

    //execute sql statement
    rc = sqlite3_exec(db, sql, callback, 0, &zErr);
    if( rc!=SQLITE_OK ){
        fprintf(stderr, "sql error: %s\n", zErr);
        sqlite3_free(zErr);
    }else{
        fprintf(stderr, "records callback successfully\n");
    }
    sqlite3_close(db);
    return 0;
}