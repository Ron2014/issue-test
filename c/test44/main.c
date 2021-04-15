/*
 * @Author: Ron
 * @LastEditors: Ron
 * @FilePath: \issue-test\c\test44\main.c
 */

#include <stdio.h>
#include <string.h>

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
    }

    return 0;
}