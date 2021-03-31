/*
 * @Author: Ron
 * @LastEditors: Ron
 * @FilePath: \issue-test\c\test40\main.c
 */
#include <stdio.h>
int main(int argc, char *argv[])
{
    printf("This is a \033[1;35m test \033[0m!\n");
    printf("This is a \033[1;32;43m test \033[0m!\n");
    printf("\033[1;33;44mThis is a test !\033[0m\n");
    return 0;
}