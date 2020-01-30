#include <stdio.h>
#include <wchar.h>
int main(void) {
	char str[] = "中文";
	wchar_t wstr[] = L"中文";
	printf("1:%s\n", str);
	wprintf(L"2:%s\n", wstr);
	return 0;
}
