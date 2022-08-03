#include <stdio.h>

int main(int argc, char *artv[])
{
    union
    {
        char a[10];
        int i;
    } u = {0};
    int i;
    int *p = (int *)&(u.a[1]);
    *p = 7;     // 对 int 来讲，p 对应的地址是未对齐的

    printf("%zd\n", sizeof(u));
    printf("%p\n", p);

    for (i = 0; i < 10; ++i)
    {
        printf("0x%02x ", u.a[i]);
    }
    printf("\n");
    return 0;
}