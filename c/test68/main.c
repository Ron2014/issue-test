#include <stdio.h>

int main(int argc, char *argv[])
{
    char a[] = "hello world";
    char *b = a;
    char *c = a;

    printf("%c\n", *(b++));
    printf("%c\n", *c++);
    return 1;
}