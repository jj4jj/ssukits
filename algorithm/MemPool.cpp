
#include "base/Log.h"
#include "base/CommonMacro.h"
#include "MemPool.h"

////////////////////BITMAP OR FREE LIST//////////////////


//          GET         FREE        MEM(1M)
// BMP     10us/1ns      1ns         125K
//LIST     1ns           1ns         4M

//based on free list .
//node - node - node
//freelist
//MemPoolHead
    //FreeListHead
    //UseHead
    //EntrySize
    //MaxNum
    //DATA BEGIN OFFSEET
//MemPoolData
    //--------[alien]
    //iNext(Free/Use)
    //magic checking
#pragma pack(1)
#define MemPoolEntryAlianSize   (8)
#define MemPoolMagic            ("mempool:magic")
struct MemPoolHead
{
    int iFreeHead;
    int iUseHead;
    int iUseTail;
    int iEntryAlianedSize;
    int iMaxNum;
    int iUsed; 
    size_t  zFootOffSet;
};
#define MemPoolHeadSize (((sizeof(MemPoolHead) + MemPoolEntryAlianSize - 1)/MemPoolEntryAlianSize)*MemPoolEntryAlianSize)
struct MemPoolEntryTag
{
    uint32_t    dwTagAndNext;
    uint32_t    dwPrev;
};
#define MEMPOOL_ENTRY_USED_MASK  (0x80000000)
struct MemPoolFoot
{
    char szMagic[32];
};
#pragma pack()


static int  GetEntryAlienedSize(int iEntrySize)
{
    int iEntryAlianSize = ((sizeof(MemPoolEntryTag) + iEntrySize + MemPoolEntryAlianSize - 1)/MemPoolEntryAlianSize)*MemPoolEntryAlianSize;
    return iEntryAlianSize;
}
/////////////////////////////////////////////////////////
int      MemPool::Init(void* pBuffer,size_t zBufferSize,
             int iEntrySize,int iMaxEntryNum)
{
    size_t zSz = GetBufferSize(iEntrySize,iMaxEntryNum);
    if(zBufferSize < zSz)
    {
        //not enough mem
        return -1;
    }
    ///////////////////////////////////////////////////
    MemPoolHead * pHead = (MemPoolHead*)pBuffer;
    memset(pBuffer,0,zBufferSize);
    int iAlianedEntrySize =  GetEntryAlienedSize(iEntrySize);
    MemPoolEntryTag * pTag = NULL;
    for(size_t i = 0; i < (size_t)iMaxEntryNum; ++i)
    {
        pTag = (MemPoolEntryTag *)((char*)pBuffer + i*iAlianedEntrySize + MemPoolHeadSize);
        if(i + 1 <  (size_t)iMaxEntryNum )
        {
            pTag->dwTagAndNext = (i+1);
        }
        else
        {
            pTag->dwTagAndNext = i;
        }
    }
    pHead->iFreeHead = 0;
    pHead->iUseHead = -1;
    pHead->iUseTail = -1;
    pHead->iEntryAlianedSize = iAlianedEntrySize;
    pHead->iMaxNum = iMaxEntryNum;
    pHead->zFootOffSet = MemPoolHeadSize + iMaxEntryNum * iAlianedEntrySize;
    MemPoolFoot * pFoot = (MemPoolFoot *)((char*)pBuffer + pHead->zFootOffSet);
    STRNCPY(pFoot->szMagic,sizeof(pFoot->szMagic),MemPoolMagic);
    ////////////////////////////////////////////////////////////////////////////////////////
    m_pBufferBase = pBuffer;
    return 0;
}
/////////////////////////////////////////////////////////////////////////////////////////////////
int     MemPool::Attach(void* pBuffer,size_t zBufferSize,
         int iEntrySize,int iMaxEntryNum)
{
    size_t zSz = GetBufferSize(iEntrySize,iMaxEntryNum);
    if(zBufferSize < zSz)
    {
        //not enough mem
        return -1;
    }
    ////////////////////////////////////////////////////////////////
    int ret = Check(pBuffer,iEntrySize,iMaxEntryNum);
    if(!ret)
    {
        m_pBufferBase = pBuffer;
    }
    return ret;
}
size_t  MemPool::GetBufferSize(int iEntrySize,int iMaxEntryNum)
{
    int iEntryAlianSize = GetEntryAlienedSize(iEntrySize);
    size_t zSize = MemPoolHeadSize + sizeof(MemPoolFoot)+iMaxEntryNum*iEntryAlianSize;
    assert(zSize%MemPoolEntryAlianSize == 0);
    return zSize;
}
//return idx
int     MemPool::Alloc()
{
    MemPoolHead * pHead = (MemPoolHead*)m_pBufferBase;
    int iFreePos = pHead->iFreeHead;
    if(iFreePos < 0)
    {
        LOG_ERROR("no more space to alloc !");
        return -1;
    }
    MemPoolEntryTag * pTag = ( MemPoolEntryTag *)((char*)m_pBufferBase + MemPoolHeadSize + pHead->iEntryAlianedSize * pHead->iFreeHead);
    assert(0 == (pTag->dwTagAndNext&MEMPOOL_ENTRY_USED_MASK));
    //remove first free node from free list
    if(pTag->dwTagAndNext == (uint32_t)pHead->iFreeHead)
    {
        //last one ; uniq one
        pHead->iFreeHead = -1;        
    }
    else
    {
        //next one
        pHead->iFreeHead = pTag->dwTagAndNext;
    }
    //////////////////////////////////////////////////////////////
    pTag->dwTagAndNext = (MEMPOOL_ENTRY_USED_MASK&iFreePos);//USED
    if(pHead->iUseTail < 0)
    {
        //first one
        pHead->iUseHead = pHead->iUseTail = iFreePos;        
        pTag->dwPrev = iFreePos;
    }
    else
    {
        pTag->dwPrev = pHead->iUseTail;
        //insert to use list tail
        MemPoolEntryTag * pUseTailTag = ( MemPoolEntryTag *)((char*)m_pBufferBase + MemPoolHeadSize + pHead->iEntryAlianedSize * pHead->iUseTail);        
        assert(pUseTailTag->dwTagAndNext == (MEMPOOL_ENTRY_USED_MASK&(pHead->iUseTail)));        
        pUseTailTag->dwTagAndNext = pTag->dwTagAndNext;
    }
    
    pHead->iUsed++;

    return iFreePos;        
}
int     MemPool::Free(int idx)
{    
    MemPoolHead * pHead = (MemPoolHead*)m_pBufferBase;
    if(pHead->iUseTail < 0 ||
       idx < 0 || idx >= pHead->iMaxNum )
    {
        LOG_ERROR("free error idx = %d",idx);
        return -1;
    }

    MemPoolEntryTag * pTag = (MemPoolEntryTag *)((char*)m_pBufferBase + MemPoolHeadSize + pHead->iEntryAlianedSize * idx);
    if(!((pTag->dwTagAndNext )& MEMPOOL_ENTRY_USED_MASK))
    {
        LOG_ERROR("free pos = %d is free !",idx);
        return -2;
    }

    //remove pos from use list
    int iNextPos = pTag->dwTagAndNext&(MEMPOOL_ENTRY_USED_MASK);
    int iPrevPos = pTag->dwPrev;

    if(iPrevPos == idx && iNextPos == idx)
    {
        pHead->iUseHead = pHead->iUseTail = -1;
    }
    else
    if(iPrevPos == idx && iNextPos != idx)
    {        
        //prev head
        pHead->iUseHead = iNextPos;
        //next pos prev
        ((MemPoolEntryTag*)((char*)m_pBufferBase + pHead->iEntryAlianedSize*iNextPos + MemPoolHeadSize))->dwPrev = iNextPos;         
    }
    else
    if(iPrevPos != idx && iNextPos == idx)
    {
        //no next node , but prev
        pHead->iUseTail = iPrevPos;
        ((MemPoolEntryTag*)((char*)m_pBufferBase + pHead->iEntryAlianedSize*iPrevPos + MemPoolHeadSize))->dwTagAndNext = iPrevPos&MemPoolHeadSize;         
    }
    //prev and next is valid
    else
    {
        ((MemPoolEntryTag*)((char*)m_pBufferBase + pHead->iEntryAlianedSize*iPrevPos + MemPoolHeadSize))->dwTagAndNext = iNextPos&MemPoolHeadSize;         
        ((MemPoolEntryTag*)((char*)m_pBufferBase + pHead->iEntryAlianedSize*iNextPos + MemPoolHeadSize))->dwPrev = iPrevPos;         
    }
    
    //insert to free list head
    pTag->dwTagAndNext = pHead->iFreeHead;
    pHead->iFreeHead = idx;

    pHead->iUsed--;

    return 0;
}
//idx
int     MemPool::Addr2Idx(void* p)
{
    MemPoolHead * pHead = (MemPoolHead*)m_pBufferBase;
    ptrdiff_t offset = (char*)p - ((char*)m_pBufferBase + MemPoolHeadSize);    
    return offset/pHead->iEntryAlianedSize;
}
void*   MemPool::Idx2Addr(int idx)
{
    MemPoolHead * pHead = (MemPoolHead*)m_pBufferBase;
    if(idx <0 || idx >= pHead->iMaxNum)
    {
        return NULL;
    }
    return ((char*)m_pBufferBase + MemPoolHeadSize + pHead->iEntryAlianedSize*idx + sizeof(MemPoolEntryTag));
}
void*   MemPool::operator [](int idx)
{
    return Idx2Addr(idx);
}
//////////////////////////
int     MemPool::MaxCount()
{
    MemPoolHead * pHead = (MemPoolHead*)m_pBufferBase;
    return pHead->iMaxNum;
}
int     MemPool::Count()
{
    MemPoolHead * pHead = (MemPoolHead*)m_pBufferBase;
    return pHead->iUsed;
}
int     MemPool::GetEntryBegin()
{
    MemPoolHead * pHead = (MemPoolHead*)m_pBufferBase;
    return pHead->iUseHead;
}
void*   MemPool::GetNextEntry(int & itr)
{
    MemPoolHead * pHead = (MemPoolHead*)m_pBufferBase;
    if(itr < 0 || itr >= pHead->iMaxNum)
    {
        return NULL;
    }
    MemPoolEntryTag* pTag = (MemPoolEntryTag*)m_pBufferBase + MemPoolHeadSize + pHead->iEntryAlianedSize* itr;
    if(pTag->dwTagAndNext & MEMPOOL_ENTRY_USED_MASK)
    {
        itr = pTag->dwTagAndNext&(~MEMPOOL_ENTRY_USED_MASK);
        return ((char*)pTag + sizeof(MemPoolEntryTag));
    }
    return NULL;
}
int     MemPool::Check(void* pBuffer,int iEntrySize,int iMaxEntryNum)
{
    MemPoolHead* pHead = (MemPoolHead*)pBuffer;
    if(pHead->iMaxNum != iEntrySize ||
       pHead->iEntryAlianedSize != GetEntryAlienedSize(iEntrySize))
    {
        //size check error
        return -2;
    }
    MemPoolFoot * pFoot = (MemPoolFoot*)((char*)pBuffer + pHead->zFootOffSet);
    if(strcmp(pFoot->szMagic,MemPoolMagic) != 0)
    {
        //magic check error
        return -3;
    }
    if(pHead->iUsed > iMaxEntryNum ||
       pHead->iUsed < 0 )
    {
        //use check error
        return -4;
    }
    return 0;
}

