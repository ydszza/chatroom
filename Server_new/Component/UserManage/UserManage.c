#include "UserManage.h"



/*
 *注册账号
 *参数：用户信息指针
 *成功：0，失败：-1
 */
int register_user(user_info* info)
{
    if(!info) {
        printf("param is NULL in %s\n", __func__);
        return -1;
    }
    if(sqlite3_insert(info) != 0){
        printf(" register user failed\n");
        return -1;
    }
    return 0;
}

/**
 * 登录验证,验证成功在传入参数结构体填入昵称
 * 参数：用户信息结构体地址
 * 成功：0，失败：-1
 */
int sign_in(user_info* info)
{
    if(!info){
        printf("param is NULL in %s\n", __func__);
        return -1;
    }
    user_info* result = sqlite3_find(info->account);
    if(!result){
        printf("user inexistent\n");
        return -1;
    }
    //核对密码
    if(strcmp(info->password, result->password) != 0){
        //printf("user password mismatch\n");
        return -1;
    }else{
        printf("user password is match\n");
        strcpy(info->nick_name, result->nick_name);
        free(result);
        result = NULL;
    }
    return 0;    
}

/**
 * 注销账号
 * 参数：用户信息结构体地址
 * 成功：0，失败：-1
 */
int unregister_user(user_info* info)
{
     if(!info){
        printf("param is NULL in %s\n", __func__);
        return -1;
     }
     if(sqlite3_delete(info) != 0){
         printf("delete user info failed\n");
         return -1;
     }
     return 0;
}

/**
 * 修改密码或昵称
 * 参数：用户信息结构体
 * 成功：0，失败：-1
 */
int updata_user_info(user_info* info)
{
    if(!info){
        printf("param is NULL in %s\n", __func__);
        return -1;
    }
    int ret = -1;
    if(info->password[0] == 0){
        ret = sqlite3_update(info, NICK_NAME);
    }else if(info->nick_name[0] == 0){
        ret = sqlite3_update(info, PASSWORD);
    }else{
        //其他情况不处理
    }
    if(ret != 0){
        printf("update user info failed\n");
    }
    return ret;
}

#if 0
/*test code*/
void test(char* test_name)
{
    if(!test_name){
        printf("param is NULL in %s\n", __func__);
        return;
    }
    printf("%s\n", test_name);
    user_info info = {
        "1454865804",
        "1234567890",
        "ydszze"
    };
    do{
        //注册
        //无效数据
        if(register_user(NULL) != -1){
            break;
        }
        printf("1\n");
        //正常数据
        if(register_user(&info) != 0){
            break;
        }
        printf("2\n");
        //登录
        //有效数据
        if(sign_in(&info) != 0){
            break;
        }
        
        printf("3\n");
        //错误密码
        info.password[0] = 5;
        if(sign_in(&info) != -1){
            break;
        }
        printf("4\n");
        //修改密码
        memset(info.nick_name, 0, sizeof(info.nick_name));
        if(updata_user_info(&info) != 0){
            break;
        }
        printf("5\n");
        //删除账号
        if(unregister_user(&info) != 0){
            break;
        }
        printf("6\n");
    }while(0);
}

int main(void)
{
    test("test1:");
    return 0;
}

#endif