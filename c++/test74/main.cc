#include <iostream>
using namespace std;

int foo(int &p)
{
    // for (int i = 0; i < 10; i ++)
    // {
    //     printf("hello\n");
    // }
    // cout << p << endl;
    return 0;
}

int main(int argc, char *argv[])
{
    int *p = NULL;
    foo(*p);
    return 0;
}