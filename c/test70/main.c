#include <stdio.h>
#include <string.h>

typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;

#define ARRAY_SAFE_COPY(szTargetBuff, iTargetBuffLen, szSrcBuff, iSrcLen)                                             \
    ({                                                                                                                \
        if (iSrcLen > iTargetBuffLen)                                                                                 \
        {                                                                                                             \
            printf("ARRAY_SAFE_COPY len out, SrcLen:%d TargetBuffLen:%d\n", (int)iSrcLen, (int)sizeof(szTargetBuff)); \
            return -1;                                                                                                \
        }                                                                                                             \
        memcpy(szTargetBuff, szSrcBuff, (int)iSrcLen);                                                                \
        iSrcLen;                                                                                                      \
    })

int main(int argc, char *argv[])
{
    uint32_t a[32] = { 1, 2, 3 };
    uint64_t b[64] = { 4, 5, 6 };

    // 把 b[0] b[1] b[2] 拷贝到 a[1] 开始的内存空间（保留 a[0] = 1）
    int size = ARRAY_SAFE_COPY(a + 1, sizeof(a) - sizeof(a[0]), b, 3 * sizeof(b[0]));
    printf("used size = %d\n", size);

    // a + 1 可以正常把目标指针移到 &a[1] 的位置
    // sizeof(a) 得到的是数组长度 32 * sizeof(uint32_t) = 128
    // 但是 sizeof(a + 1) 的值是8 (指针长度) 而不是理想值 (32 - 1) * sizeof(uint32_t) = 124
    // 替代方案：
    // sizeof(a) - sizeof(a[0])
    // (sizeof(a)/sizeof(a[0]) - 1) * sizeof(a[0])

    for (int i = 0; i < 32; i++ )
    {
        printf("%d\n", a[i]);
    }

    // 尝试 0 拷贝调用 memcpy
    // size = ARRAY_SAFE_COPY(a, sizeof(a), b, 0);
    // printf("it works...\n");

    memcpy(a, b, 0);
    printf("it works...\n");
    return 0;
}