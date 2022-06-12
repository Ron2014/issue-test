#include <iostream>
#include <stddef.h>
using namespace std;

template <typename T, size_t N>
char (&ArraySizeHelper(const T (&array)[N]))[N];

#define ARRAYSIZE(array) (sizeof(ArraySizeHelper(array)))

int main(int argc, char *argv[])
{
    int arrint[128];
    cout << ARRAYSIZE(arrint) << " " << sizeof(ARRAYSIZE(arrint)) << endl;
    return 0;
}