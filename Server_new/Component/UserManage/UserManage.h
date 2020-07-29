#ifndef __USERMANAGE_H
#define __USERMANAGE_H

#include "../Sqlite3/Sqlite3.h"
#include <stdio.h>

int register_user(user_info* info);
int sign_in(user_info* info);
int unregister_user(user_info* info);
int updata_user_info(user_info* info);


#endif // !__USERMANAGE_H