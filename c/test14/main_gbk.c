#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <wchar.h>

int main(void) {
    setlocale(LC_ALL, "zh_CN.UTF-8");

    char str[] = "中文";
    wchar_t wstr[] = L"中文";
    printf("1:%s\n", str);
    wprintf(L"2:%s\n", wstr);
    // wprintf(L"%c", L'你');
	fprintf(stdout, "3:%s\n", str);
    return 0;
}