#include <memory>
#include <cstring>

typedef struct tagMemoryPool
{
    int iSize;
    int iFreeBlockHead;
    char *szData;
} MemoryPool, MEMORYPOOL, *LPMEMORYPOOL;

typedef struct tagMemoryBlock
{
    int iSize;
    int iPrevFree;  // >=0 上一个空块；-1 没有空块了；-2 使用中
    int iNextFree;  // >=0 下一个空块；-1 没有空块了；-2 使用中
    char szData[1];
} MemoryBlock, MEMORYBLOCK, *LPMEMORYBLOCK;

MemoryPool g_MemoryPool;

MemoryBlock *GetFreeBlock(int iOffset)
{
    if (iOffset >= g_MemoryPool.iSize)
    {
        return NULL;
    }
    return (MemoryBlock *)(g_MemoryPool.szData + iOffset);
}

int InitMemoryPool(int iSize)
{
    if (iSize <= 0)
    {
        return -1;
    }
    g_MemoryPool.szData = (char *)malloc(iSize);
    if (g_MemoryPool.szData == NULL)
    {
        return -1;
    }
    memset(g_MemoryPool.szData, 0, iSize);
    g_MemoryPool.iSize = iSize;
    g_MemoryPool.iFreeBlockHead = 0;

    MemoryBlock *pstFirstBlock = GetFreeBlock(0);
    pstFirstBlock->iSize = iSize;
    pstFirstBlock->iNextFree = -1;
    pstFirstBlock->iPrevFree = -1;

    return 0;
}

int FreeMemory(LPMEMORYBLOCK pstBlock)
{
    int iOffset = (char *)pstBlock - g_MemoryPool.szData;
    if (iOffset + pstBlock->iSize + sizeof(MemoryBlock) < g_MemoryPool.iSize)
    {
        // 如果后面是空块就合并
        LPMEMORYBLOCK pstNextBlock = LPMEMORYBLOCK((char *)pstBlock + pstBlock->iSize);
        if (pstNextBlock->iNextFree >= 0)
        {
            pstBlock->iSize += pstNextBlock->iSize;

            if(pstNextBlock->iPrevFree >= 0)
            {
                GetFreeBlock(pstNextBlock->iPrevFree)->iNextFree = pstNextBlock->iNextFree;
            }

            if (pstNextBlock->iNextFree >= 0)
            {
                GetFreeBlock(pstNextBlock->iNextFree)->iPrevFree = pstNextBlock->iPrevFree;
            }
        }
    }

    LPMEMORYBLOCK pstPrevBlock = GetFreeBlock(g_MemoryPool.iFreeBlockHead);
    pstPrevBlock->iPrevFree = iOffset;

    pstBlock->iPrevFree = -1;
    pstBlock->iNextFree = g_MemoryPool.iFreeBlockHead;
    g_MemoryPool.iFreeBlockHead = iOffset;
    return 0;
}

int AllocMemory(int iSize, LPMEMORYBLOCK *ppResult)
{
    int iNeedSize = iSize + sizeof(MemoryBlock);
    LPMEMORYBLOCK pstBlock = GetFreeBlock(g_MemoryPool.iFreeBlockHead);
    LPMEMORYBLOCK pstPrevBlock = NULL;

    while (pstBlock->iSize < iNeedSize)
    {
        if (pstBlock->iNextFree <= 0)
        {
            return -1;
        }
        pstPrevBlock = pstBlock;
        pstBlock = GetFreeBlock(pstBlock->iNextFree);
    }

    if (pstBlock == NULL)
    {
        return -1;
    }

    int iLeftSize = pstBlock->iSize - iNeedSize;
    if (iLeftSize > sizeof(MemoryBlock))
    {
        MemoryBlock *pNewFreeBlock = LPMEMORYBLOCK((char *)pstBlock + iNeedSize);
        pNewFreeBlock->iSize = iLeftSize;
        pNewFreeBlock->iNextFree = pstBlock->iNextFree;

        if (pstPrevBlock)
        {
            pstPrevBlock->iNextFree += iNeedSize;
        }
    }
    else
    {
        if (pstPrevBlock)
        {
            pstPrevBlock->iNextFree = pstBlock->iNextFree;
        }
    }
    pstBlock->iPrevFree = -2;
    pstBlock->iNextFree = -2;

    if (ppResult)
    {
        *ppResult = pstBlock;
    }

    return 0;
}