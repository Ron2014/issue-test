#include <stdlib.h>
#include <stdio.h>

int RandI(int a, int b)
{
    if (a > b)
    {
        int tmp = b;
        b = a;
        a = tmp;
    }

    if (a >= b)
    {
        return b;
    }

    // if (a < 0)
    // {
    //     return a + RandI(0, b - a);
    // }
    // else
    // {
    //     return a + (int)((b - a + 1) * (rand() / (RAND_MAX + 1.0)));
    // }

    return a + (int)((b - a + 1) * (rand() / (RAND_MAX + 1.0)));
}

int RandII(int a, int b)
{
    if (a > b)
    {
        int tmp = b;
        b = a;
        a = tmp;
    }

    if (a >= b)
    {
        return b;
    }

    // [0, b - a] -> [a, b]
    return a + rand() % (b - a + 1);
}

float RandF(float a, float b)
{
    if (a > b)
    {
        float tmp = b;
        b = a;
        a = tmp;
    }

    if (a >= b)
    {
        return b;
    }

    // if (a < 0)
    // {
    //     return a + RandI(0, b - a);
    // }
    // else
    // {
    //     return a + (float)((b - a + 1) * (rand() / (RAND_MAX + 1.0)));
    // }

    return a + ((b - a) * (rand() / (RAND_MAX + 0.0)));
}

int main(int argc, char *argv[]) {
    printf("%d\n", rand());
    printf("%d\n", rand());
    printf("%d\n", rand());

    printf("%d\n", rand());
    printf("0x%x\n", RAND_MAX);

    if (argc < 3)
    {
        printf("usage: main a b\n");
        return 1;
    }

    for (int i = 0; i < 10; i++)
    {
        printf("rand(%s, %s)=%d\n", argv[1], argv[2], RandI(atoi(argv[1]), atoi(argv[2])));
    }

    return 0;
}