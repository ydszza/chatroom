#ifndef __CONFIG_H
#define __CONFIG_H

/*IP和端口号*/
#define IP_ADDR                 "127.0.0.1"
#define IP_PORT                 12345

/*线程个数自定义范围*/
#define THREAD_MAX_NUM          50
#define THREAD_MIN_NUM          5
/*线程池的动态调节步长，即一次 增加/减少 的线程数*/
#define THREAD_STEP             5

/*任务队列自定义最大值*/
#define TASKS_MAX_NUM           50

/*定义sqlite3数据库的位置*/
#define PATH    "./ChatRoom.db"

#endif // !__CONFIG_H