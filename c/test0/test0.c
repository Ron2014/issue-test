#include <stdio.h>
#include <ctype.h>
#include <stddef.h>

int main(int argc, char *argv[]) {
    char a[] = {
        0xFF, 0x8F, 0x7F, 0x3F, 0x1F, 0x0F, 0xF9, 0x79, 0xB9,
        'a',
        '0', '9', '-1',
        '\0'
    };
    char *b = a;

    while(*b) {
        // unsigned int count = (b-a)/sizeof(b);
        ptrdiff_t count = b - a;
        char c = *b;
        unsigned char uc = (unsigned char)(c);
        int ci = (int)c;
        int uci = (int)uc;

        printf("a[%td]:\n", count);
        printf("char %c\n", c);
        printf("unsigned char %c\n", uc);
        printf("int for char %d(0x%x)\n", ci, ci);
        printf("int for unsigned char %d(0x%x)\n", uci, uci);

        // isdigit 0x30 ~ 0x39 (0011 0000 ~ 0011 1001)
        printf("isdigit for char: %d\n", isdigit(ci));
        printf("isdigit for unsigned char: %d\n", isdigit(uci));
        
        printf("---\n");
        b++;
    }
    return 0;
}