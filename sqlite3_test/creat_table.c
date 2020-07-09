#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>

static int callback(void *NotUsed, int argc, char **argv, char **azColName)
{
    int i;
    for (i = 0; i < argc; i++){
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
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

    //open the database
    rc = sqlite3_open("test.db", &db);
    if( rc){
        fprintf(stderr, "Error opening database: %s\n", sqlite3_errmsg(db));
        exit(1);
    }else{
        fprintf(stderr, "open database successfully\n");
    }

    //create sql statement
    sql = "CREATE TABLE COMPANY (" \
            "ID INT PRIMARY KEY     NOT NULL, " \
            "NAME           TEXT    NOT NULL, " \
            "AGE            INT     NOT NULL, " \
            "ADDRESS        CHAR(50)," \
            "SALARY         REAL);";
    
    //execute sql statement
    rc = sqlite3_exec(db, sql, callback, 0, &zErr);
    if( zErr != SQLITE_OK ){
        fprintf(stderr, "Error executing: %s\n", zErr);
        sqlite3_free(zErr);
    }else{
        fprintf(stderr, "table create successfully\n");
    }
    sqlite3_close(db);
    return 0;
}