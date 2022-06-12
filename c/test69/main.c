#include <stdio.h>

unsigned int HashmapCode(void *pObj, int iSize)
{
    unsigned long long uiHash = 0;
    unsigned int uiSeed = 131;

    char *szKey = (char *)pObj;
    while(iSize-- > 0)
    {
        uiHash = uiHash * uiSeed + (int)*szKey++;
    }
    return (unsigned int)(uiHash & 0xFFFFFFFF);
}

int main(int argc, char *argv[])
{
    int a = 0x1234;
    char *b = (char *)&a;
    int testa[32] = {0};
    int *testb;
    printf("%x\n", *b);
    printf("%x\n", (int)*b);
    printf("%x\n", *(int *)b);
    printf("%x\n", (char)*(int *)b);
    printf("%d\n", sizeof(testa));
    printf("%d\n", sizeof(testb));
    return 0;
}