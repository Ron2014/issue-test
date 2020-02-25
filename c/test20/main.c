#include <stdio.h>

int main(int argc, char *argv[]) {
    printf("size: pointer %zd\n", sizeof(void*));
    printf("size: long %zd\n", sizeof(long));
    printf("size: int %zd\n", sizeof(int));
    printf("size: long unsigned int %zd\n", sizeof(long unsigned int));
    printf("size: size_t %zd\n", sizeof(size_t));
    return 0;
}