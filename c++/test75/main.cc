#include <iostream>
using namespace std;

int main(int argc, char *argv[])
{
    uint64_t a = 0xFFFFFFFFFFFFFFFF;
    uint64_t b = a + 1;
    uint64_t c = b - a;
    cout << a << endl;
    cout << b << endl;
    cout << c << endl;

    return 0;
}