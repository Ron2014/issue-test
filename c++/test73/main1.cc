#include <iostream>
#include <cstring>
#include <stdlib.h>
#include <stdio.h>
#include <map>
using namespace std;

void showResult(int *arr, int n, int end)
{
    // 列举所有数字，会有重复，会有0
    for (int i = 0; i < n; i ++)
    {
        cout << (i < end ? arr[i] : 0) << " ";
    }
    cout << endl;

    // // 不为0的数字:数量
    // // 测试证明，这种结果表示，字节数据更大
    // map<int, int> result;
    // for (int i = 0; i < n && i < end; i++)
    // {
    //     ++result[arr[i]];
    // }
    // for (auto &it : result)
    // {
    //     cout << it.first << ":" << it.second << " ";
    // }
    // cout << endl;
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

    for (int i = arr[pos-1]; i <= m; i++)
    {
        arr[pos] = i;
        foo(m-i, n, arr, pos+1);
    }
}

void printResult(int m, int n)
{
    int *arr = new int[n];

    for (int i = 1; i <= m; i++)
    {
        memset(arr, 0, sizeof(int)*n);
        arr[0] = i;
        foo(m-i, n, arr, 1);
    }

    delete arr;
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