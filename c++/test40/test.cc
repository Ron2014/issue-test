/*
 * @Author: Ron
 * @LastEditors: Ron
 * @FilePath: \issue-test\c++\test40\test.cc
 */
#include <iostream>
#include <windef.h>
using namespace std;

int main(int argc, char *argv[])
{
    int a = 2147483647;
    cout << a + 1 << endl;
    printf("%d\n", a + 1);

    cout << sizeof(BOOL) << endl;
    cout << sizeof(bool) << endl;

    return 0;
}