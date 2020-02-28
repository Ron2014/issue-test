#include <stdio.h>
#include <string.h>

#define STR_TEST "string"

int main(int argc, char *argv[]) {
    printf("size: pointer %zd\n", sizeof(void*));
    printf("size: int %zd\n", sizeof(int));
    printf("size: short %zd\n", sizeof(short));
    printf("size: long %zd\n", sizeof(long));
    printf("size: long long %zd\n", sizeof(long long));
    printf("size: size_t %zd\n", sizeof(size_t));
    printf("size: long unsigned int %zd\n", sizeof(long unsigned int));
    printf("size: const char* in macro %zd, len %zd\n", sizeof(STR_TEST), strlen(STR_TEST));
    return 0;
}