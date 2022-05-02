#include <iostream>
#include <cstring>
#include <map>
using namespace std;

// #define SHOW_MAP
#define SIZE_BUF 1024
char buffer[SIZE_BUF];

typedef struct Result
{
    uint32_t dwTotal;
    uint32_t dwSlotNum;
    struct Result *pstWithNone;
    struct Result *pstFullSlot;
    bool bPureOne;
} Result, *LPRESULT;

#define GLOABLID_TYPE uint64_t
map<GLOABLID_TYPE, Result> mapResult;

GLOABLID_TYPE getKey(uint32_t m, uint32_t n)
{
    uint64_t ullKey = uint64_t(m);
    ullKey <<= (sizeof(m)*8);
    ullKey |= n;
    return ullKey;
}

char *Key2Str(uint64_t ullKey)
{
    uint32_t m = uint32_t(ullKey >> sizeof(uint32_t)*8);
    uint32_t n = uint32_t(ullKey & (uint32_t)-1);
    snprintf(buffer, sizeof(buffer), "%u->%u", m, n);
    return buffer;
}

void showResult(uint32_t *arr, uint32_t slot)
{
    for (uint32_t i = 0; i < slot; i++)
    {
        cout << arr[i] << " ";
    }
    cout << endl;
}

void printResult(uint32_t *arr, uint32_t iTotalSlot, LPRESULT pstResult)
{
    if (!pstResult)
    {
        showResult(arr, iTotalSlot);
        return;
    }

    int dwTotal = pstResult->dwTotal;
    int dwSlotNum = pstResult->dwSlotNum;

    if (dwSlotNum == 1)
    {
        arr[dwSlotNum - 1] += dwTotal;
        showResult(arr, iTotalSlot);
        arr[dwSlotNum - 1] -= dwTotal;
        return;
    }

    if (pstResult->pstWithNone)
    {
        printResult(arr, iTotalSlot, pstResult->pstWithNone);
    }

    if (pstResult->bPureOne)
    {
        for (int i = 0; i < dwSlotNum && i < dwTotal; i++)
        {
            ++arr[i];
        }
        printResult(arr, iTotalSlot, pstResult->pstFullSlot);
        for (int i = 0; i < dwSlotNum && i < dwTotal; i++)
        {
            --arr[i];
        }
    }
}

void calcResult(uint32_t m, uint32_t n)
{
    for (uint32_t i = 1; i <= m; i++)
    {
        for (uint32_t j = 1; j <= n; j ++)
        {
            GLOABLID_TYPE key = getKey(i, j);
            Result &r = mapResult[key];
            r.dwTotal = i;
            r.dwSlotNum = j;
            r.pstWithNone = NULL;
            r.pstFullSlot = NULL;
            r.bPureOne = false;

            if (i < j)
            {
                // 直接取 <i, i> 的表示方法
                Result &r1 = mapResult[getKey(i, i)];
                r.pstWithNone   = r1.pstWithNone;
                r.pstFullSlot   = r1.pstFullSlot;
                r.bPureOne      = r1.bPureOne;
            }
            else
            {
                // 至少有一个空的情况
                r.pstWithNone = (j > 1) ? &mapResult[getKey(i, j - 1)] : NULL;
                // 铺满j的情况
                r.bPureOne = true;
                // 若铺满j还有剩余
                r.pstFullSlot = (i > j) ? &mapResult[getKey(i - j, j)] : NULL;
            }
        }
    }

    uint32_t *arr = new uint32_t[n];
    memset(arr, 0, sizeof(uint32_t)*n);
    printResult(arr, n, &mapResult[getKey(m, n)]);
    delete [] arr;
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

    calcResult(m, n);

#ifdef SHOW_MAP
    for (auto &it: mapResult)
    {
        cout << Key2Str(it.first) << "------" << endl;
        cout << it.second.dwTotal << endl;
        cout << it.second.dwSlotNum << endl;
        cout << it.second.bPureOne << endl;
        if (it.second.pstWithNone)
        {
            cout << "withNone :" << it.second.pstWithNone->dwTotal << "->" << it.second.pstWithNone->dwSlotNum << endl;
        }
        if (it.second.pstFullSlot)
        {
            cout << "fullSlot :" << it.second.pstFullSlot->dwTotal << "->" << it.second.pstFullSlot->dwSlotNum << endl;
        }
    }
#endif

    return 0;
}
