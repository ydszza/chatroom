#include "Sqlite3.h"

/**
 * 功能：查询结果回调函数
 * 参数
 * 返回值
*/
static int callback(void *data, int argc, char **argv, char **azColName)
{
    //printf("callback\n");
    user_info** info = (user_info**)data;
    //int i;
    // for (i = 0;i<argc;i++) {
    //     printf("%s = %s\n", azColName[i],argv[i] ? argv[i] : "NULL");
    //     strcpy(data, argv[i]);
    // }
    // printf("\n");
    if(argc >= 3) {   
        *info = (user_info*)malloc(sizeof(user_info));
        if(!info){
            return 0;
        }
        memset((*info), 0, sizeof(user_info));    
        strcpy((*info)->account, argv[0]);
        strcpy((*info)->password, argv[1]);
        strcpy((*info)->nick_name, argv[2]);
    }
    return 0;
}

/**
 * 功能：查找数据
 * 参数：
 * 返回值：
 */
user_info* sqlite3_find(char *account)
{
    if(account == NULL){
        printf("sqlite3 find func invalid input!\n");
        return NULL;
    }
    //sql操作语句
    char sql[1024];
    sprintf(sql, "select * from USER_INFO where account = %s;", account);
    //printf("sqlite3 find func \n%s\n", sql);
    sqlite3 *db;
    char *zErr = NULL;
    int rc;

    //打开数据库
    rc = sqlite3_open(PATH, &db);
    if( rc!=SQLITE_OK ){
        fprintf(stderr, "sqlite3 find func open database error: %s\n", sqlite3_errmsg(db));
        return NULL;
    }
    
    //执行查询语句
    user_info* info = NULL;
    rc = sqlite3_exec(db, sql, callback, &info, &zErr);
    if( rc!=SQLITE_OK ){
        fprintf(stderr, "sqlite3 find func sql error: %s\n", zErr);
        sqlite3_free(zErr);
        sqlite3_close(db);
        return NULL;
    }else{
        fprintf(stderr, "sqlite3 find func recode database successfully\n");
    }
    sqlite3_close(db);
    return info;
}

/**
 * 功能：插入数据
 * 参数：个人信息结构体
 * 返回值：成功返回0，失败返回-1
*/
int sqlite3_insert(user_info* info)
{
    if(info == NULL){
        printf("sqlite3 insert func invalid input!\n");
        return -1;
    }

    sqlite3 *db;
    char *zErr = NULL;
    int rc;
    //打开数据库
    rc = sqlite3_open(PATH, &db);
    if( rc!=SQLITE_OK ){
        fprintf(stderr, "sqlite3 insert func open database error: %s\n", sqlite3_errmsg(db));
        return -1;
    }
    
    //sql操作语句
    char sql[1024];
    sprintf(sql, "insert into USER_INFO values ('%s','%s','%s');", info->account, info->password, info->nick_name);
    //printf("sqlite3 insert func sql:\n%s\n", sql);

    //执行插入语句
    rc = sqlite3_exec(db, sql, NULL, NULL, &zErr);
    if( rc!=SQLITE_OK ){
        fprintf(stderr, "sqlite3 insert func sql error: %s\n", zErr);
        sqlite3_free(zErr);
        sqlite3_close(db);
        return -1;
    }else{
        fprintf(stderr, "sqlite3 insert func recode database successfully\n");
    }
    sqlite3_close(db);
    return 0;
}

/**
 * 功能：修改数据
 * 参数：
 * 返回值：失败返回-1，成功返回0
*/
int sqlite3_update(user_info* info, enum mode mod)
{
    if(info == NULL){
        printf("sqlite3 update func invalid input!\n");
        return -1;
    }else if(mod == PASSWORD || mod == NICK_NAME){
        //修改密码或昵称
        sqlite3 *db;
        char *zErr = NULL;
        int rc;

        //打开数据库
        rc = sqlite3_open(PATH, &db);
        if( rc!=SQLITE_OK ){
            fprintf(stderr, "sqlite3 updata func open database error: %s\n", sqlite3_errmsg(db));
            return -1;
        }
        
        //sql操作语句
        char sql[1024];
        if(mod == PASSWORD){
            sprintf(sql, "update USER_INFO set password = '%s' where account = '%s';",
                        info->password, info->account);
        }else{
            sprintf(sql, "update USER_INFO set nickname = '%s' where account = '%s';",
                        info->nick_name, info->account);
        }
        //printf("sqlite3 updata func sql:\n%s\n", sql);

        //执行修改语句
        rc = sqlite3_exec(db, sql, NULL, NULL, &zErr);
        if( rc!=SQLITE_OK ){
            fprintf(stderr, "sqlite3 updata func sql error: %s\n", zErr);
            sqlite3_free(zErr);
            sqlite3_close(db);
            return -1;
        }else{
            fprintf(stderr, "sqlite3 updata func recode database successfully\n");
        }
        sqlite3_close(db);
        return 0;
    }else{
        //其他情况不操作
        return -1;
    }
}

/*删除记录
 *参数
 *返回值：失败返回-1，成功返回0
 */
int sqlite3_delete(user_info* info)
{
    if(info == NULL){
        printf("sqlite3 delete func invalid input!\n");
        return -1;
    }

    sqlite3 *db;
    char *zErr = NULL;
    int rc;
    //打开数据库
    rc = sqlite3_open(PATH, &db);
    if( rc!=SQLITE_OK ){
        fprintf(stderr, "sqlite3 delete func open database error: %s\n", sqlite3_errmsg(db));
        return -1;
    }
    
    //sql操作语句
    char sql[1024];
    sprintf(sql, "delete from USER_INFO where account = '%s'", info->account);
    //printf("sqlite3 delete func sql:\n%s\n", sql);

    //执行插入语句
    rc = sqlite3_exec(db, sql, NULL, NULL, &zErr);
    if( rc!=SQLITE_OK ){
        fprintf(stderr, "sqlite3 delete func sql error: %s\n", zErr);
        sqlite3_free(zErr);
        sqlite3_close(db);
        return -1;
    }else{
        fprintf(stderr, "sqlite3 delete func recode database successfully\n");
    }
    sqlite3_close(db);
    return 0;
}

#if 0
/*test code*/
void test(char* test_name, user_info* info)
{
    if(!info || !test_name){
        printf("test func invalid input!\n");
        return;
    }
    printf("%s: \n", test_name);
    user_info* result;
    sqlite3_insert(info);
    result = sqlite3_find(info->account);
    printf("account = %s\tpassword = %s\tnick_name = %s\n",
            result->account, result->password, result->nick_name);
    free(result);
    result = NULL;
    strcpy(info->nick_name, "ydsqqq");
    sqlite3_update(info, NICK_NAME);
    result = sqlite3_find(info->account);
    printf("account = %s\tpassword = %s\tnick_name = %s\n",
            result->account, result->password, result->nick_name);
    free(result);
    result = NULL;
    sqlite3_delete(info);
}
int main()
{
    user_info info = {"1454865804", "qwer12304", "ydszze"};
    test("test1", &info);
    return 0;
}
#endif