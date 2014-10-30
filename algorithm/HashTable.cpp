
#include "base/CommonMacro.h"
#include "base/Log.h"
#include "math/PrimeTable.h"
#include "HashTable.h"

//format 
//key value struct
//key -> size_t
//value -> membuffer

//mempool + hash 
//key -> idx


//HASHTABLEHEAD
//KEYMAP
//MEMPOOL

#define HashTableAlianSize   (8)
#pragma pack(1)
struct HashTableKeyIndex
{
    size_t  zKey;
    int     iIndex;
};
struct HashTableHead
{
    int             iTotalBucketsNum;
    int             iEntrySize;
    int             iMaxEntryNum;
    int             iHashTableKeyIndexListTail;
    int             iTotalUseCount;
    /////////////////////////////////////////////////////////////////////
    int             iMaxBucketsFactor;
    int             aKeyBucketsNum[HashTable::DEFAULT_MAX_BUCKETS_FACTOR];
    size_t          azKeyBucketsOffset[HashTable::DEFAULT_MAX_BUCKETS_FACTOR];
    size_t          zKeyIndexListOffset;
    size_t          zmmpoolOffset;
    char            szMagic[32];
};
#pragma pack()

#define HashTableMagic  ("hashtable_magic")
#define HashTableHeadSize       ((sizeof(HashTableHead) + HashTableAlianSize - 1 )/HashTableAlianSize*HashTableAlianSize)
#define HashTableKeyIndexSize   ((sizeof(HashTableKeyIndex) + HashTableAlianSize - 1)/HashTableAlianSize*HashTableAlianSize)

#if 1
static HashTableKeyIndex * FindListKeyIndex(HashTableHead* pHead,size_t zhash,
                                            PFNHashCompare comp,
                                            void * pEntry,MemPool & mempool,
                                            int   iBegin )
{
    LOG_WARN("find in list !");
    //todo replace by bsearch
    HashTableKeyIndex * pIndex = (HashTableKeyIndex*)((char*)pHead + pHead->zKeyIndexListOffset);
    assert(pHead->iHashTableKeyIndexListTail < pHead->iMaxEntryNum);
    if(iBegin >= pHead->iHashTableKeyIndexListTail)
    {
        return NULL;
    }
    for(int i = iBegin; i < pHead->iHashTableKeyIndexListTail; ++i)
    {
        if(pIndex[i].zKey != zhash)
        {
            continue;
        }
        void * prEntry = mempool.Idx2Addr(pIndex[i].iIndex);
        if(comp(prEntry,pEntry) == 0)
        {
            return &(pIndex[i]);
        }
    }
    return NULL;    
}
static int      InsertListKeyIndex(HashTableHead* pHead,size_t zhash,
                        PFNHashCompare comp,MemPool & mempool,
                        void * pEntry )
{

    LOG_WARN("insert in list !");

    //todo replace by bsearch
    HashTableKeyIndex * pIndex = (HashTableKeyIndex*)((char*)pHead + pHead->zKeyIndexListOffset);
    assert(pHead->iHashTableKeyIndexListTail < pHead->iMaxEntryNum);
    for(int i = 0;i < pHead->iHashTableKeyIndexListTail; ++i)
    {
        if(pIndex[i].zKey == zhash)
        {
            void* prEntry =  mempool.Idx2Addr(pIndex[i].iIndex);
            if(0 == comp(pEntry,prEntry))
            {
                //repeat
                return -2;
            }        
        }
    }
    //////////////////////////////////////////////////////////
    int iPos =  mempool.Alloc();
    memcpy(mempool.Idx2Addr(iPos),pEntry,pHead->iEntrySize);
    ++(pHead->iTotalUseCount);
    pIndex[pHead->iHashTableKeyIndexListTail].iIndex = iPos;
    pIndex[pHead->iHashTableKeyIndexListTail].zKey = zhash;    
    ++(pHead->iHashTableKeyIndexListTail);
    return 0;
}

static int  GetListKeyIndexOffset(HashTableHead* pHead,HashTableKeyIndex * pIndex )
{
    HashTableKeyIndex * pBase = (HashTableKeyIndex* )((char*)pHead + pHead->zKeyIndexListOffset);
    if(pIndex < pBase ||
       pIndex > pBase + pHead->iHashTableKeyIndexListTail )
    {
        LOG_FATAL("get error offset base = %p p = %p!",pBase,pIndex);
        return -1;
    }
    return  (pIndex - pBase)/HashTableKeyIndexSize;    
}

static int  RemoveListKeyIndex(HashTableHead* pHead,HashTableKeyIndex * pIndex,MemPool & mempool)
{
    int iOffset = GetListKeyIndexOffset(pHead,pIndex);
    if(iOffset < 0)
    {
        return -1;
    }
    mempool.Free(pIndex->iIndex);
    --pHead->iTotalUseCount;
    HashTableKeyIndex * pBase = (HashTableKeyIndex* )((char*)pHead + pHead->zKeyIndexListOffset);
    pBase[iOffset] = pBase[pHead->iHashTableKeyIndexListTail - 1];
    pBase[pHead->iHashTableKeyIndexListTail - 1].iIndex = -1;
    --(pHead->iHashTableKeyIndexListTail);
    return 0;
}

#endif


//
//iMaxEntryNum
//f(iMaxEntryNum,1)
//..
//f(iMaxEntryNum,iMaxBucketsFactor)
//iMaxEntryNum

//size = head + sizeof(int)*(f(iMaxEntryNum,1...iMaxBucketsFactor)+iMaxEntryNum)+mmpool


//a hash table flat in a memory block
#if 1
int     HashTable::Init(void* pBuffer,size_t zBufferSize,
             int iEntySize,int iMaxEntryNum,
             PFNHashCode hash,
             PFNHashCompare comp,
             int iMaxBucketsFactor )
{
    if(iMaxBucketsFactor < 1 || iMaxBucketsFactor > DEFAULT_MAX_BUCKETS_FACTOR)
    {
        return -1;
    }
    size_t zSize = GetBufferSize(iEntySize, iMaxEntryNum, iMaxBucketsFactor);
    if(zBufferSize < zSize)
    {
        //error size
        return -2;
    }
    //////////////////////////////////////////////////////////////////////////
    memset(pBuffer,0,zSize);
    HashTableHead * pHead = (HashTableHead*)pBuffer;
    int iLayerBuckets = 0;
    OnTheFlyPrimeTable  ofp;
    int iLastPrime = ofp.GetNextPrime(iMaxEntryNum);
    iLayerBuckets += iLastPrime;
    pHead->iTotalBucketsNum = 0;
    pHead->iEntrySize = iEntySize;
    pHead->iMaxBucketsFactor = iMaxBucketsFactor;
    pHead->aKeyBucketsNum[0] = iLastPrime;
    pHead->azKeyBucketsOffset[0] = HashTableHeadSize;    
    pHead->iTotalBucketsNum += iLastPrime;
    for(int i = 1;i <= iMaxBucketsFactor; ++i)
    {
        int  p = ofp.GetPrevPrime(iLastPrime);
        if(p <= 0)
        {
            p = iLastPrime;
        }
        iLayerBuckets += p;
        iLastPrime = p;
        pHead->iTotalBucketsNum += iLastPrime;
        pHead->aKeyBucketsNum[i] = iLastPrime;
        pHead->azKeyBucketsOffset[i] = 
            pHead->azKeyBucketsOffset[i-1] + pHead->aKeyBucketsNum[i-1]*sizeof(int);
    }
    for(int j = 0 ; j < iLastPrime; ++j)
    {
        ((int*)((char*)m_pBuffer + HashTableHeadSize ))[j] = -1;
    }
    pHead->zKeyIndexListOffset = pHead->azKeyBucketsOffset[iMaxBucketsFactor-1] + pHead->aKeyBucketsNum[iMaxBucketsFactor-1]*sizeof(int);
    pHead->iTotalBucketsNum += iMaxEntryNum;
    for(int j = 0 ; j < iMaxEntryNum; ++j)
    {
        HashTableKeyIndex * pKeyIndex = (HashTableKeyIndex*)((char*)m_pBuffer + pHead->zKeyIndexListOffset);
        pKeyIndex[j].iIndex = -1;
    }    
    pHead->zmmpoolOffset = pHead->zKeyIndexListOffset + iMaxEntryNum*HashTableKeyIndexSize;

    STRNCPY(pHead->szMagic,sizeof(pHead->szMagic),HashTableMagic);
    size_t zMemPoolSize = zSize - pHead->zmmpoolOffset;
    if(mempool.Init(pBuffer,zMemPoolSize, iEntySize, iMaxEntryNum))
    {
        LOG_ERROR("mem pool init error !");
        return -3;
    }
    ////////////////////////////////////////////////
    m_fnHash = hash;
    m_fnComp = comp;
    m_pBuffer = pBuffer;
    return 0;
}
/////////////////////////////////////////////////////////////////////////////////////////////////
int     HashTable::Attach(void* pBuffer,size_t zBufferSize,
             int iEntrySize,int iMaxEntryNum,
             PFNHashCode hash,
             PFNHashCompare comp,
             int iMaxBucketsFactor )
{
    
    size_t zSz = GetBufferSize(iEntrySize,iMaxEntryNum);
    if(zBufferSize < zSz)
    {
        //not enough mem
        return -1;
    }
    ////////////////////////////////////////////////////////////////
    int ret = Check(pBuffer,iEntrySize,iMaxEntryNum,iMaxBucketsFactor);
    if(!ret)
    {
        m_pBuffer = pBuffer;
        m_fnHash = hash;
        m_fnComp = comp;
    }
    return ret;    
}
size_t  HashTable::GetBufferSize(int iEntySize,int iMaxEntryNum,int iMaxBucketsFactor )
{
    size_t sz = 0;
    if(iMaxBucketsFactor < 1 || iMaxBucketsFactor > DEFAULT_MAX_BUCKETS_FACTOR)
    {
        return 0;
    }
    OnTheFlyPrimeTable     ptble;    
    int p = ptble.GetNextPrime(iMaxEntryNum);
    sz = p;
    for(int i = 1;i < iMaxBucketsFactor; ++i)
    {
        int pp = ptble.GetPrevPrime(p);
        if(pp < 0)
        {
            pp = p;
        }
        sz += pp;
        p = pp;
    }
    sz *= sizeof(int);
    sz += iMaxEntryNum*HashTableKeyIndexSize;
    sz += HashTableHeadSize;
    sz += MemPool::GetBufferSize(iEntySize,iMaxEntryNum);    
    return sz;
}

int     HashTable::Insert(void* pEntry)
{   
    size_t zhash = m_fnHash(pEntry);
    HashTableHead * pHead = (HashTableHead*)m_pBuffer;    
    if(pHead->iTotalUseCount >= pHead->iMaxEntryNum)
    {
        return -1;
    }
    //multi factor hash
    for(int i = 0;i < pHead->iMaxBucketsFactor; ++i)
    {
        int idx = zhash%pHead->aKeyBucketsNum[i];
        int * pIndexBase = (int*)((char*)m_pBuffer + pHead->azKeyBucketsOffset[i]);
        if(pIndexBase[idx] < 0)
        {
            //alloc it
            //mempool insert            
            int iPos = mempool.Alloc();
            memcpy(mempool.Idx2Addr(iPos),pEntry,pHead->iEntrySize);            
            pIndexBase[idx] = iPos;
            pHead->iTotalUseCount++;            
            return 0;
        }
        ///if is same , repeat
        void* prEntry =  mempool.Idx2Addr(pIndexBase[idx]);
        if(0 == m_fnComp(pEntry,prEntry))
        {
            //repeat
            return -2;
        } 
    }
    //all layer is full , insert to tail
    return InsertListKeyIndex(pHead,zhash,m_fnComp,mempool,pEntry);
}
int     HashTable::Remove(void* pEntry)
{
    size_t zhash = m_fnHash(pEntry);
    HashTableHead * pHead = (HashTableHead*)m_pBuffer;
    int iFindLayer  = -1;
    int * piFindIdx = NULL;
    for(int i = 0;i < pHead->iMaxBucketsFactor; ++i)
    {
        int idx = zhash%pHead->aKeyBucketsNum[i];
        int * pIndexBase = (int*)((char*)m_pBuffer + pHead->azKeyBucketsOffset[i]);
        if(pIndexBase[idx] < 0)
        {
            //not found
            return -1;
        }
        //find a same
        void* prEntry =  mempool.Idx2Addr(pIndexBase[idx]);
        if(m_fnComp(pEntry,prEntry) == 0)
        {
            iFindLayer = i;
            piFindIdx = ((int*)((char*)m_pBuffer + pHead->azKeyBucketsOffset[i])) + idx;        
            break;
        }
    }

    if(iFindLayer < 0)
    {
        return -1;
    }
    ////////////////////////////////////////////////////////////////////////////////////
    assert(piFindIdx);
    int * piTailIdx = piFindIdx;
    //swap with tail
    int iLayerTail = pHead->iMaxBucketsFactor - 1;
    for(; iLayerTail > iFindLayer; --iLayerTail)
    {
        int tidx = zhash%pHead->aKeyBucketsNum[iLayerTail];
        piTailIdx = ((int*)((char*)m_pBuffer + pHead->azKeyBucketsOffset[iLayerTail])) + tidx;
        if(*piTailIdx >= 0)
        {
            break;
        }
    }    
    //at least = i;
    assert(piTailIdx);
    int iRemoveIndex = *piFindIdx;
    //not last layer
    if( iLayerTail < pHead->iMaxBucketsFactor - 1)
    {
        *piFindIdx = *piTailIdx;
        *piTailIdx = -1;
    }
    //last layer
    else
    {
        //find out the entry
        int iBegin = 0;
        HashTableKeyIndex * pIndex = FindListKeyIndex(pHead,zhash,m_fnComp,pEntry,mempool,iBegin);
        //not found
        if(!pIndex)
        {
            *piFindIdx = *piTailIdx;
            *piTailIdx = -1;
        }
        else
        {
            //swap 
            *piFindIdx = pIndex->iIndex;
            pIndex->iIndex = iRemoveIndex;
            //remove list
            return RemoveListKeyIndex(pHead,pIndex,mempool);
        }
    }

    //remove in layer buckets 
    mempool.Free(iRemoveIndex);
    --pHead->iTotalUseCount;
    
    return 0;
}
void*   HashTable::Find(void* pEntry)
{
    size_t zhash = m_fnHash(pEntry);
    HashTableHead * pHead = (HashTableHead*)m_pBuffer;
    for(int i = 0;i < pHead->iMaxBucketsFactor; ++i)
    {
        int idx = zhash%pHead->aKeyBucketsNum[i];
        int * pIndexBase = (int*)((char*)m_pBuffer + pHead->azKeyBucketsOffset[i]);
        if(pIndexBase[idx] < 0)
        {
            //not found
            return NULL;
        }
        //find a same
        void* prEntry =  mempool.Idx2Addr(pIndexBase[idx]);
        if(m_fnComp(pEntry,prEntry) == 0)
        {
            return prEntry;
        }
    }

    HashTableKeyIndex * pIndex = FindListKeyIndex( pHead, zhash,m_fnComp,pEntry,mempool,0);
    if(!pIndex)
    {
        return NULL;
    }
    return mempool.Idx2Addr(pIndex->iIndex);
}
//////////////////////////
int     HashTable::MaxCount()
{
    HashTableHead * pHead = (HashTableHead*)m_pBuffer;
    return pHead->iMaxEntryNum;
}
int     HashTable::Count()
{
    HashTableHead * pHead = (HashTableHead*)m_pBuffer;
    return pHead->iTotalUseCount;
}
//head is 0
int     HashTable::GetBeinEntry()
{
    return mempool.GetEntryBegin();
}
void*   HashTable::GetNextEntry(int & itr)
{
    return mempool.GetNextEntry(itr);
}
int     HashTable::Check(void* pBuffer,int iEntrySize,int iMaxEntryNum,int iMaxFactor)
{
    HashTableHead* pHead = (HashTableHead*)pBuffer;
    if(pHead->iMaxEntryNum != iMaxEntryNum ||
       pHead->iMaxBucketsFactor != iMaxFactor )
    {
        //size check error
        return -2;
    }
    if(strcmp(pHead->szMagic,HashTableMagic) != 0)
    {
        //magic check error
        return -3;
    }
    if(pHead->iTotalUseCount > iMaxEntryNum ||
       pHead->iTotalUseCount < 0 )
    {
        //use check error
        return -4;
    }
    return MemPool::Check(((char*)pBuffer + pHead->zmmpoolOffset),iEntrySize,iMaxEntryNum);
}

#endif
    
