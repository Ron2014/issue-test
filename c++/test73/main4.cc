#include <iostream>
#include <cstring>
using namespace std;

void showResult(uint32_t *arr, uint32_t slot)
{
    for (uint32_t i = 0; i < slot; i++)
    {
        cout << arr[i] << " ";
    }
    cout << endl;
}

void printResult(uint32_t *arr, uint32_t iTotalSlot, uint32_t dwTotal, uint32_t dwSlotNum)
{
    if (dwTotal == 0)
    {
        showResult(arr, iTotalSlot);
        return;
    }

    if (dwSlotNum == 1)
    {
        arr[dwSlotNum - 1] += dwTotal;
        showResult(arr, iTotalSlot);
        arr[dwSlotNum - 1] -= dwTotal;
        return;
    }

    if (dwTotal < dwSlotNum)
    {
        printResult(arr, iTotalSlot, dwTotal, dwTotal);
    }
    else
    {
        // 至少有一个空的情况
        printResult(arr, iTotalSlot, dwTotal, dwSlotNum - 1);

        // 铺满的情况
        for (int i = 0; i < dwSlotNum && i < dwTotal; i++)
        {
            ++arr[i];
        }
        
        // 如果铺满还有剩，递归
        printResult(arr, iTotalSlot, dwTotal - dwSlotNum, dwSlotNum);

        for (int i = 0; i < dwSlotNum && i < dwTotal; i++)
        {
            --arr[i];
        }
    }
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

    if (m <= 0 || n <= 0)
    {
        printf("m or n <=0\n");
        return -1;
    }

    uint32_t *arr = new uint32_t[n];
    memset(arr, 0, sizeof(uint32_t)*n);
    printResult(arr, n, m, n);
    delete [] arr;

    return 0;
}