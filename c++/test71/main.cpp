#include <iostream>
using namespace std;

int main(int argc, char* argv[])
{
    char szData[20] = { 'h', 'e', 'l', 'l', 'o'};

    printf("%p\n", szData);
    printf("%p\n", &szData);
    printf("%p\n", &szData[0]);

    printf("%s\n", szData);
    printf("%s\n", &szData);
    printf("%s\n", &szData[0]);

    return 0;
}
