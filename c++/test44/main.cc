/*
 * @Date: 2021-06-03 15:08:58
 * @Author: Ron
 * @LastEditors: Ron
 * @LastEditTime: 2021-06-03 15:28:33
 * @FilePath: \issue-test\c++\test44\main.cc
 * @Description: 
 */
#include <iostream>
#include <string>
#include <cstring>
using namespace std;

int main(int argc, char *argv[])
{
    if (argc <= 1) {
        printf("usage: proc str1 str2 str3\n");
        return 1;
    }

    int i;
    for (i = 1; i <argc; i++)
    {
        printf("strlen(\'%s\')=%d\n", argv[i], strlen(argv[i]));
        printf("sizeof(\'%s\')=%d\n", argv[i], sizeof(argv[i]));

        std::string str1 = argv[i];
        printf("(\'%s\').size()=%zd\n", argv[i], str1.size());
        printf("(\'%s\').length()=%zd\n", argv[i], str1.length());
    }

    char str2[] = "hello";
    printf("strlen(\'%s\')=%d\n", str2, strlen(str2));
    printf("sizeof(\'%s\')=%d\n", str2, sizeof(str2));

    const char *str3 = "world";
    printf("strlen(\'%s\')=%d\n", str3, strlen(str3));
    printf("sizeof(\'%s\')=%d\n", str3, sizeof(str3));

    return 0;
}