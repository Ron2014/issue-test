#include <iostream>
#include <cstring>
#include <stdlib.h>
#include <stdio.h>
using namespace std;

void showResult(int *arr, int n, int end)
{
    for (int i = 0; i < n; i ++)
    {
        cout << (i < end ? arr[i] : 0) << " ";
    }
    cout << endl;
}

void foo(int m, int n, int *arr, int pos)
{
    if (m < 0 || pos > n)
    {
        return;
    }

    if (m == 0)
    {
        showResult(arr, n, pos);
        return;
    }

    if (pos == n)
    {
        return;
    }

    // 保证降序
    for (int i = arr[pos-1]; i >= 0; i--)
    {
        arr[pos] = i;
        foo(m-i, n, arr, pos+1);
    }
}

void printResult(int m, int n)
{
    int *arr = new int[n];

    for (int i = m; i >= 0; i--)
    {
        memset(arr, 0, sizeof(int)*n);
        arr[0] = i;
        foo(m-i, n, arr, 1);
    }

    delete[] arr;
}

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        printf("usage: proc M N\n");
        return -1;
    }

    int m = atoi(argv[1]);
    int n = atoi(argv[2]);
    printResult(m, n);
    return 0;
}
