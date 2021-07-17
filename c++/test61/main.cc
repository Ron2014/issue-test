/*
 * @Date: 2021-07-10 13:16:17
 * @Author: Ron
 * @LastEditors: Ron
 * @LastEditTime: 2021-07-10 13:28:55
 * @FilePath: \issue-test\c++\test61\main.cc
 * @Description: 
 */
#include <iostream>
#include <stdio.h>

#define MAX_BUF_LEN 128
char mBuffer[MAX_BUF_LEN] = {0};

void foo0(const char *info)
{
    printf("%s\n", info);
}

char *foo1(const char *msg)
{
    snprintf(mBuffer, MAX_BUF_LEN, "this is key:%s", msg);
    return mBuffer;
}

int main(int argc, char *argv[])
{
    foo0("hello world");
    foo0(foo1("0001"));
    foo0(foo1("1"));
    return 0;
}