#include <stdio.h>
#define SET_VALUE(i) ((i)=((i)|0x1))

int main(int argc, char *argv[]) {
    int a = 10;
    int b[3];
    b[0] = 0;
    b[1] = 2;
    b[2] = 4;
    int *c = b + 1;
    SET_VALUE(*c);
    printf("%d\n", b[1]);
    return 0;
}