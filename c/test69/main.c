#include <stdio.h>

int main(int argc, char *argv[])
{
    int a = 0x1234;
    char *b = (char *)&a;
    printf("%x\n", *b);
    printf("%x\n", (int)*b);
    printf("%x\n", *(int *)b);
    printf("%x\n", (char)*(int *)b);
    return 0;
}