/*
 * @Date: 2021-07-05 10:54:45
 * @Author: Ron
 * @LastEditors: Ron
 * @LastEditTime: 2021-07-05 16:30:17
 * @FilePath: \issue-test\c++\test56\main.cc
 * @Description: 
 */

#include <iostream>
#include <cstring>
using namespace std;

#define UINT32 unsigned char
#define UINT64 unsigned long long
#define MAX_LEN_REDIS_KEY 64

struct A
{
    UINT32 ID;
};

struct B
{
    UINT64 ID;
};

enum STRUCT_TYPE
{
    STRUCT_A,
    STRUCT_B,
};

/** 
 * 模板函数的特化
 * 只有一个CUtils，公共的缓冲区
 */
#define DECLARE_REDIS_KEY_GENERATOR(TName, TKey, Enum) \
    template <>                                        \
    char *CUtils::GetRedisKey<TName>(TKey id)          \
    {                                                  \
        memset(CUtils::mBuffer, 0, MAX_LEN_REDIS_KEY); \
        sprintf(CUtils::mBuffer, "%04u:%u", Enum, id); \
        return CUtils::mBuffer;                        \
    }

#define DECLARE_REDIS_KEY_CREATOR(TName, KeyName)                \
    template <>                                                  \
    char *CUtils::CreateSubscribeKey<TName>(TName & data) \
    {                                                            \
        return CUtils::GetRedisKey<TName>(data.KeyName);         \
    }

class CUtils
{
private:
    static char mBuffer[MAX_LEN_REDIS_KEY];
public:
    template<typename T>
    static char *GetRedisKey(UINT32 id);
    template<typename T>
    static char *GetRedisKey(UINT64 id);
    template<typename T>
    static char *CreateSubscribeKey(T &data);
};

char CUtils::mBuffer[] = {0};

DECLARE_REDIS_KEY_GENERATOR(A, UINT32, STRUCT_TYPE::STRUCT_A)
DECLARE_REDIS_KEY_GENERATOR(B, UINT64, STRUCT_TYPE::STRUCT_B)

DECLARE_REDIS_KEY_CREATOR(A, ID)
DECLARE_REDIS_KEY_CREATOR(B, ID)

/** 
 * 模板类的特化
 * 每个类对应一个CUtils_: 这里很冗余，于其这样不如写到每个类里面 T::GetRedisKey
 * CUtils_有独立的缓冲区
 */
#define DECLARE_CUTILS(TName, TKey, KeyName, Enum)        \
    template <>                                           \
    char CUtils_<TName>::mBuffer[] = {0};                 \
    template <>                                           \
    char *CUtils_<TName>::GetRedisKey(TKey id)            \
    {                                                     \
        memset(CUtils_::mBuffer, 0, MAX_LEN_REDIS_KEY);   \
        sprintf(CUtils_::mBuffer, "%04u:%u", Enum, id);   \
        return CUtils_::mBuffer;                          \
    }                                                     \
    template <>                                           \
    char *CUtils_<TName>::CreateSubscribeKey(TName &data) \
    {                                                     \
        return CUtils_<TName>::GetRedisKey(data.KeyName); \
    }

template<typename T>
class CUtils_
{
private:
    static char mBuffer[MAX_LEN_REDIS_KEY];
public:
    static char *GetRedisKey(UINT32 id);
    static char *GetRedisKey(UINT64 id);
    static char *CreateSubscribeKey(T &data);
};

DECLARE_CUTILS(A, UINT32, ID, STRUCT_TYPE::STRUCT_A)
DECLARE_CUTILS(B, UINT64, ID, STRUCT_TYPE::STRUCT_B)

int main(int argc, char *argv[])
{
    A a;
    B b;

    printf("CUtils_<A>::CreateSubscribeKey(a) = %s\n", CUtils_<A>::CreateSubscribeKey(a));
    printf("CUtils_<B>::CreateSubscribeKey(b) = %s\n", CUtils_<B>::CreateSubscribeKey(b));

    printf("CUtils::CreateSubscribeKey(a) = %s\n", CUtils::CreateSubscribeKey(a));
    printf("CUtils::CreateSubscribeKey(b) = %s\n", CUtils::CreateSubscribeKey(b));

    return 0;
}