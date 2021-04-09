/*
 * @Author: Ron
 * @LastEditors: Ron
 * @FilePath: \issue-test\c\test42\test2.c
 */

#include <stdio.h>
#include <stdlib.h>

int main()
{
    // FILE *stream;
    printf("%lx %zd %d\n", stdout, sizeof(*stdout), stdout->_fileno);
    FILE *f = freopen("file1.txt", "w", stdout);
    printf("%lx %lx %p %zd\n", stdout, f, f, sizeof(*f));

    size_t len = sizeof(*f);
    unsigned char *buf = (unsigned char*)f;
    while(len--)
    {
        printf("%d ", *buf);
        buf++;
    }
    printf("\n%d %d %d %d\n", f->_flags & _IONBF, f->_flags & _IOFBF, f->_flags & _IOLBF, f->_fileno);

    printf("this is in file1.txt"); // 这句话在file1.txt中显示
    freopen("CON", "w", stdout);
    printf("And this is in command.\n"); //这句话在控制台上显示
    return 0;
}