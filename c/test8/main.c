#include <stdio.h>
#define SET_VALUE(i) ((i)=((i)|0x1))

typedef unsigned int Instruction;

// void swap(int &a, int &b) {
//     int tmp = a;
//     a = b;
//     b = tmp;
// }

void foo(Instruction **p_codelist) {
    Instruction *a = *p_codelist, b, *c;

    printf("%zd %zd %zd %zd\n", sizeof(Instruction), sizeof(a), sizeof(b), sizeof(c));
}

int main(int argc, char *argv[]) {
    int a = 10;
    int b[3];
    b[0] = 0;
    b[1] = 2;
    b[2] = 4;
    int *c = b + 1;
    SET_VALUE(*c);
    printf("%d\n", b[1]);

    // printf("%d %d\n", b[0], b[1]);
    // swap(b[0], b[1]);
    // printf("%d %d\n", b[0], b[1]);

    Instruction d = 10;
    Instruction *e = &d;
    foo(&e);
    return 0;
}