/*
 * @Date: 2021-08-12 23:46:11
 * @Author: Ron
 * @LastEditors: Ron
 * @LastEditTime: 2021-08-13 00:15:18
 * @Description: 
 */

/*
 gcc -o main main.cc -lhiredis -Wformat
*/

#include <string.h>
#include <iostream>
#include <cstdlib>
#include <string>
#include "hiredis/hiredis.h"

using namespace std;

int main()
{
    //1、连接Redis服务器
    redisContext *context = redisConnect("127.0.0.1", 6379);
    if (context->err)
    {
        std::cout << "can not connect to redisserver: 127.0.0.1, port:6379" << std::endl;
        std::cout << "reason:" << context->errstr << std::endl;
        redisFree(context);
        context = NULL;
        return 0;
    }
 
    //2、校验密码
    const char *auth = "123456";
    redisReply *reply = (redisReply *)redisCommand(context, "AUTH %s", auth);
    if (NULL == reply)
    {
        std::cout << "wrong password!" << std::endl;
        freeReplyObject(reply);
        redisFree(context);
        context = NULL;
        return 0;
    }
    freeReplyObject(reply);
 
    const char value[] = {0xff, 0x01, 0x05, 0x66, 0xfd, 0x00};
    
    //3、选择数据库 - 不选择也可
    reply = (redisReply *)redisCommand(context, "SET key0 %s", value);
    if (NULL == reply || REDIS_REPLY_ERROR == reply->type)
    {
        std::cout << "SET key0 failed!" << std::endl;
        freeReplyObject(reply);
        redisFree(context);
        context = NULL;
        return 0;
    }
    freeReplyObject(reply);

    int argc = 3;

    const char *argv[argc];
    argv[0] = "SET";
    argv[1] = "key1";
    argv[2] = value;

    size_t lens[argc];
    lens[0] = strlen(argv[0]);
    lens[1] = strlen(argv[1]);
    lens[2] = sizeof(value);
 
    reply = (redisReply *)redisCommandArgv(context, argc, argv, lens);
    if (NULL == reply || REDIS_REPLY_ERROR == reply->type)
    {
        std::cout << "SET key1 failed!" << std::endl;
        freeReplyObject(reply);
        redisFree(context);
        context = NULL;
        return 0;
    }
    freeReplyObject(reply);

    redisFree(context);
    context = NULL;

    return 0;
}