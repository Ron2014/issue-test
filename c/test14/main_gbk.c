#include <stdio.h>
#include <wchar.h>
int main(void) {
    char str[] = "����";
    wchar_t wstr[1024] = L"����";
    printf("1:%s\n", str);
    wprintf(L"2:%s\n", wstr);
	fprintf(stdout, "3:%s\n", str);
    return 0;
}