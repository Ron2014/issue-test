#include <stdio.h>
// #include <locale.h>

int main(void) {
    // setlocale(LC_ALL, "zh_CN.gbk");
    const char str[] = "����"; // ����
    const char eng[] = "hello world";
    printf("1:%s %s\n", str, eng);
	fprintf(stdout, "2:%s %s\n", str, eng);
	fprintf(stderr, "3:%s %s\n", str, eng);
    return 0;
}