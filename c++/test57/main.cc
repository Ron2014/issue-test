/*
 * @Date: 2021-07-05 15:10:58
 * @Author: Ron
 * @LastEditors: Ron
 * @LastEditTime: 2021-07-05 20:13:07
 * @FilePath: \issue-test\c++\test57\main.cc
 * @Description: 
 */

#include <iostream>
#include <cstring>
using namespace std;

#define UINT32 unsigned char
#define UINT64 unsigned long long
#define MAX_LEN_REDIS_KEY 64

class A
{
public:
    UINT32 M1;
    UINT64 M2;
    A():M1(0),M2(1){}
};

struct B
{
public:
    UINT64 M1;
    UINT32 M2;
    B():M1(2),M2(3){}
};

enum STRUCT_TYPE
{
    STRUCT_A,
    STRUCT_B,
};

class Utils
{
public:
    template <typename T>
    static void Foo(T &data);

    template <typename TK>
    static void FooMember(TK &data);
};

template <typename TK>
void Utils::FooMember(TK &data)
{
    printf("Utils::FooMember for [%s]%llu\n", typeid(TK).name(), data);
}

template <typename T>
void Utils::Foo(T &data)
{
    Utils::FooMember<decltype(data.M1)>(data.M1);
    Utils::FooMember<decltype(data.M2)>(data.M2);
}

int main(int argc, char *argv[])
{
    A a;
    B b;
    Utils::Foo<A>(a);
    Utils::Foo<B>(b);
    printf("[%s]\n", typeid(unsigned char).name());
    printf("[%s]\n", typeid(unsigned long long).name());
    printf("[%s]\n", typeid(A).name());
    printf("[%s]\n", typeid(B).name());
    return 0;
}