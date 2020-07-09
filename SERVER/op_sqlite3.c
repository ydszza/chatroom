#include "op_sqlite3.h"

/**
 * 功能：查询结构回调函数
 * 参数
 * 返回值
*/
static int callback(void *data, int argc, char **argv, char **azColName)
{
    printf("callback\n");
    int i;
    for (i = 0;i<argc;i++) {
        printf("%s = %s\n", azColName[i],argv[i]?argv[i]:"NULL");
        memcpy(data, argv[i], 10);
    }
    printf("\n");
    return 0;
}

/**
 * 功能：插入数据
 * 参数：
 * 返回值：
 */
char *my_sqlite3_find(char *account_t)
{
    //sql操作语句
    char sql[64];
    char account[11] = {0};
    memcpy(account, account_t, 10);
    sprintf(sql, "SELECT password FROM USER_INFO where account = %s;", account);
    printf("%s\n", sql);
    sqlite3 *db;
    char *zErr = NULL;
    int rc;
    //打开数据库
    rc = sqlite3_open("./ChatRoom.db", &db);
    if( rc!=SQLITE_OK ){
        fprintf(stderr, "open database error: %s\n", sqlite3_errmsg(db));
        return NULL;
    }
    
    //执行查询语句
    char *password = (char *)malloc(11);
    memset(password, 0, 11);
    rc = sqlite3_exec(db, sql, callback, password, &zErr);
    if( rc!=SQLITE_OK ){
        fprintf(stderr, "sql error: %s\n", zErr);
        sqlite3_free(zErr);
        sqlite3_close(db);
        return NULL;
    }else{
        fprintf(stderr, "recode database successfully\n");
    }
    sqlite3_close(db);
    char cmp[10] = {0};
    if(memcmp(password, cmp, 10) == 0){
        printf("无记录\n");
        return NULL;
    }
    return password;
}

/**
 * 功能：查询数据
 * 参数：账号，密码
 * 返回值：成功返回0，失败返回-1
*/
int my_sqlite3_insert(char *account_t, char *password_t)
{
    sqlite3 *db;
    char *zErr = NULL;
    int rc;
    //打开数据库
    rc = sqlite3_open("./ChatRoom.db", &db);
    if( rc!=SQLITE_OK ){
        fprintf(stderr, "open database error: %s\n", sqlite3_errmsg(db));
        return -1;
    }
    
    //sql操作语句
    char sql[64];
    char account[11] = {0};
    memcpy(account, account_t, 10);
    sprintf(sql, "INSERT INTO USER_INFO VALUES (%s,%s);", account_t, password_t);
    printf("%s\n", sql);

    //执行插入语句
    rc = sqlite3_exec(db, sql, NULL, NULL, &zErr);
    if( rc!=SQLITE_OK ){
        fprintf(stderr, "sql error: %s\n", zErr);
        sqlite3_free(zErr);
        sqlite3_close(db);
        return -1;
    }else{
        fprintf(stderr, "recode database successfully\n");
    }
    sqlite3_close(db);
    return 0;
}

/**
 * 功能：修改数据
 * 参数：
 * 返回值：
*/
void my_sqlite3_update(char *account_t, char *password_t)
{
    //
}

// int main()
// {
//     my_sqlite3_insert("1454865805", "4567891230");
//     char cmp[10] = {0};
//     char *buf = my_sqlite3_find("1454865805");
//     if(buf == NULL){
//         printf("Error\n");
//         return -1;
//     }else{
//         printf("%s\n", buf);
//     }
//     return 0;
// }