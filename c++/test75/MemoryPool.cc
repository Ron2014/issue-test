#include "MemoryPool.h"

bool IsFreeBlock(LPMEMORYBLOCK pstBlock)
{
    return pstBlock->bUsed == false;
}

bool IsUsedBlock(LPMEMORYBLOCK pstBlock)
{
    return pstBlock->bUsed == true;
}

LPMEMORYBLOCK GetBlock(int iOffset)
{
    if (iOffset < 0 || iOffset > g_MemoryPool.iSize - SIZE_BLOCK)
    {
        return NULL;
    }

    LPMEMORYBLOCK pstBlock = LPMEMORYBLOCK(g_MemoryPool.szData + iOffset);
    return pstBlock;
}

LPMEMORYBLOCK GetFreeBlock(int iOffset)
{
    if (iOffset < 0 || iOffset > g_MemoryPool.iSize - SIZE_BLOCK)
    {
        return NULL;
    }

    LPMEMORYBLOCK pstBlock = LPMEMORYBLOCK(g_MemoryPool.szData + iOffset);
    if (!IsFreeBlock(pstBlock))
    {
        return NULL;
    }

    return pstBlock;
}

int GetBlockOffset(LPMEMORYBLOCK pstBlock)
{
    return (char *)pstBlock - g_MemoryPool.szData;
}

int InitMemoryPool(int iSize)
{
    if (iSize <= 0)
    {
        return -1;
    }

    int iNeedSize = iSize + SIZE_BLOCK;
    g_MemoryPool.szData = (char *)malloc(iNeedSize);
    if (g_MemoryPool.szData == NULL)
    {
        return -1;
    }

    memset(g_MemoryPool.szData, 0, iNeedSize);
    g_MemoryPool.iSize = iNeedSize;
    g_MemoryPool.iFreeSize = iNeedSize;
    g_MemoryPool.iFreeBlockHead = 0;
    g_MemoryPool.iFreeBlockNum = 1;
    g_MemoryPool.iUsedBlockHead = -1;
    g_MemoryPool.iUsedBlockNum = 0;

    MemoryBlock *pstFirstBlock = GetFreeBlock(0);
    pstFirstBlock->iSize = iNeedSize;
    pstFirstBlock->bUsed = false;
    pstFirstBlock->iNext = -1;
    pstFirstBlock->iPrev = -1;

    return 0;
}

int AddFreeBlock(LPMEMORYBLOCK pstBlock)
{
    int iOffset = GetBlockOffset(pstBlock);
    LPMEMORYBLOCK pstPrevBlock = GetFreeBlock(g_MemoryPool.iFreeBlockHead);
    pstPrevBlock->iPrev = iOffset;

    pstBlock->bUsed = false;
    pstBlock->iPrev = -1;
    pstBlock->iNext = g_MemoryPool.iFreeBlockHead;

    g_MemoryPool.iFreeBlockHead = iOffset;
    g_MemoryPool.iFreeBlockNum++;
    g_MemoryPool.iFreeSize += pstBlock->iSize;
    return 0;
}

int RemoveFreeBlock(LPMEMORYBLOCK pstBlock)
{
    LPMEMORYBLOCK pstTmpBlock;
    pstTmpBlock = GetFreeBlock(pstBlock->iPrev);
    if (pstTmpBlock)
    {
        pstTmpBlock->iNext = pstBlock->iNext;
    }
    else
    {
        g_MemoryPool.iFreeBlockHead = pstBlock->iNext;
    }
    pstTmpBlock = GetFreeBlock(pstBlock->iNext);
    if (pstTmpBlock)
    {
        pstTmpBlock->iPrev = pstBlock->iPrev;
    }

    g_MemoryPool.iFreeBlockNum--;
    g_MemoryPool.iFreeSize -= pstBlock->iSize;
    return 0;
}

int AddUsedBlock(LPMEMORYBLOCK pstBlock)
{
    int iOffset = GetBlockOffset(pstBlock);
    LPMEMORYBLOCK pstPrevBlock = GetFreeBlock(g_MemoryPool.iUsedBlockHead);
    if (pstPrevBlock)
    {
        pstPrevBlock->iPrev = iOffset;
    }

    pstBlock->bUsed = true;
    pstBlock->iPrev = -1;
    pstBlock->iNext = g_MemoryPool.iUsedBlockHead;

    g_MemoryPool.iUsedBlockHead = iOffset;
    g_MemoryPool.iUsedBlockNum++;
    return 0;
}

int RemoveUsedBlock(LPMEMORYBLOCK pstBlock)
{
    LPMEMORYBLOCK pstTmpBlock;
    pstTmpBlock = GetBlock(pstBlock->iPrev);
    if (pstTmpBlock)
    {
        pstTmpBlock->iNext = pstBlock->iNext;
    }
    else
    {
        g_MemoryPool.iUsedBlockHead = pstBlock->iNext;
    }
    pstTmpBlock = GetBlock(pstBlock->iNext);
    if (pstTmpBlock)
    {
        pstTmpBlock->iPrev = pstBlock->iPrev;
    }

    g_MemoryPool.iUsedBlockNum--;
    return 0;
}

int FreeMemory(LPMEMORYBLOCK pstBlock)
{
    RemoveUsedBlock(pstBlock);
    AddFreeBlock(pstBlock);

    LPMEMORYBLOCK pstTmpBlock;
    int iOffset = GetBlockOffset(pstBlock);
    LPMEMORYBLOCK pstNearBlock = GetFreeBlock(iOffset + pstBlock->iSize);
    if (pstNearBlock)
    {
        // 如果后面是空块就合并
        RemoveFreeBlock(pstNearBlock);
        pstBlock->iSize += pstNearBlock->iSize;
        g_MemoryPool.iFreeSize += pstNearBlock->iSize;
    }
    return 0;
}

int AllocMemory(int iSize, LPMEMORYBLOCK *ppResult)
{
    LPMEMORYBLOCK pstTmpBlock;
    int iNeedSize = iSize + SIZE_BLOCK;
    if (g_MemoryPool.iFreeSize < iNeedSize)
    {
        return -1;
    }

    LPMEMORYBLOCK pstBlock = GetFreeBlock(g_MemoryPool.iFreeBlockHead);

    while (pstBlock && pstBlock->iSize < iNeedSize)
    {
        if (pstBlock->iNext <= 0)
        {
            return -1;
        }
        pstBlock = GetFreeBlock(pstBlock->iNext);
    }

    if (pstBlock == NULL)
    {
        return -1;
    }

    RemoveFreeBlock(pstBlock);
    AddUsedBlock(pstBlock);

    if (ppResult)
    {
        *ppResult = pstBlock;
    }

    int iLeftSize = pstBlock->iSize - iNeedSize;
    if (iLeftSize > SIZE_BLOCK)
    {
        // 如果内存够大就分割
        pstBlock->iSize = iNeedSize;

        int iOffset = GetBlockOffset(pstBlock);
        LPMEMORYBLOCK pstNewFreeBlock = GetBlock(iOffset + iNeedSize);
        pstNewFreeBlock->iSize = iLeftSize;
        AddFreeBlock(pstNewFreeBlock);
    }

    return 0;
}
