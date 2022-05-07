#include <memory>
#include <cstring>

typedef struct tagMemoryPool
{
    int iSize;
    int iFreeSize;
    int iFreeBlockHead;
    int iFreeBlockNum;
    int iUsedBlockHead;
    int iUsedBlockNum;
    char *szData;
} MemoryPool, MEMORYPOOL, *LPMEMORYPOOL;

typedef struct tagMemoryBlock
{
    int iSize;
    bool bUsed;
    int iPrev;  // >=0 上一个空块；-1 没有空块了
    int iNext;  // >=0 下一个空块；-1 没有空块了
    char szData[1];
} MemoryBlock, MEMORYBLOCK, *LPMEMORYBLOCK;

#define SIZE_BLOCK sizeof(MemoryBlock)

MemoryPool g_MemoryPool;

bool IsFreeBlock(LPMEMORYBLOCK pstBlock);
bool IsUsedBlock(LPMEMORYBLOCK pstBlock);
LPMEMORYBLOCK GetBlock(int iOffset);
LPMEMORYBLOCK GetFreeBlock(int iOffset);
int GetBlockOffset(LPMEMORYBLOCK pstBlock);

int InitMemoryPool(int iSize);
int FreeMemory(LPMEMORYBLOCK pstBlock);
int AllocMemory(int iSize, LPMEMORYBLOCK *ppResult);

int AddFreeBlock(LPMEMORYBLOCK pstBlock);
int RemoveFreeBlock(LPMEMORYBLOCK pstBlock);

int AddUsedBlock(LPMEMORYBLOCK pstBlock);
int RemoveUsedBlock(LPMEMORYBLOCK pstBlock);