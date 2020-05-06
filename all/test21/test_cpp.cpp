#include <stdio.h>

int main(int argc, char *argv[]) {
    if (-1) {
        printf("-1 is true\n");
    }else{
        printf("-1 is false\n");
    }

    int a = 1, b = 2;
    int c = a > b ? a : b;
    printf("%d\n", c);

    return 0;
}
