/*
 * @Author: Ron
 * @LastEditors: Ron
 * @FilePath: \issue-test\c\test42\test0.c
 */

#include <stdio.h>
#include <stdlib.h>

int main()
{
    if (freopen("file.txt", "w", stdout) == NULL)
        fprintf(stderr, "error\n");
    printf("This is in the file\n"); //这句话会在file.txt中显示。
    fclose(stdout);                  //使用fclose()函数就可以把缓冲区内最后剩余的数据输出到磁盘文件中，并释放文件指针和有关的缓冲区。
    return 0;
}