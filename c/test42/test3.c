/*
 * @Author: Ron
 * @LastEditors: Ron
 * @FilePath: \issue-test\c\test42\test3.c
 */

#include <stdio.h>
#include <stdlib.h>

#define MAXLINE 1024

void pr_stdio(const char *, FILE *);
int is_unbuffered(FILE *);
int is_linebuffered(FILE *);
int buffer_size(FILE *);

int main()
{
    FILE *fp;
    char buf[MAXLINE];

    // if (setvbuf(stderr, buf, _IOLBF, MAXLINE) != 0)
    // { 
    //     /*我是萌萌哒解决办法*/
    //     fprintf(stderr, "setvbuf error\n");
    // }

    fputs("enter any char \n", stdout);
    if (getchar() == EOF)
    {
        fputs("get err ", stderr);
        exit(0);
    }
    fputs("one line to standart error\n", stderr);

    pr_stdio("stdin", stdin);
    pr_stdio("stdout", stdout);
    pr_stdio("stderr", stderr);
    if ((fp = fopen("/etc/passwd", "r")) == NULL)
    {
        fputs("fopen error\n", stderr);
        return -1;
    }
    if (getc(fp) == EOF)
    {
        fputs("getc error", stderr);
        return -1;
    }
    pr_stdio("/etc/passwd", fp);
    exit(0);
}
void pr_stdio(const char *name, FILE *fp)
{
    printf("stream is %s, ", name);

    if (is_unbuffered(fp))
        printf("unbuffered");
    else if (is_linebuffered(fp))
        printf("line buffered");
    else
        printf("fully buffered");
    printf(", buffer size = %d\n", buffer_size(fp));
}
#if defined(_IO_UNBUFFERED)

int is_unbuffered(FILE *fp)
{
    return (fp->_flags & _IO_UNBUFFERED);
}

int is_linebuffered(FILE *fp)
{
    return (fp->_flags & _IO_LINE_BUF);
}
int buffer_size(FILE *fp)
{
    return (fp->_IO_buf_end - fp->_IO_buf_base);
}
//#elif define(__SNBF)
#endif