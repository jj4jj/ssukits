

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

#pragma pack(1)
#define HashTableAlianSize   (8)
struct HashTableKeyIndex
{
    size_t  zKey;
    int     iIndex;
};
struct HashTableHead
{
    PFNHashCode     fnCode;
    PFNHashCompare  fnComp;
    int             iTotalBucketsNum;
    int             iMaxEntryNum;
    int             iHashTableKeyIndexListTail;
    int             iTotalUseCount;
    /////////////////////////////////////////////////////////////////////
    int             iMaxBucketsFactor;
    int             aBucketsFactor[HashTable::DEFAULT_MAX_BUCKETS_FACTOR];
    size_t          azKeyBucketsOffset[HashTable::DEFAULT_MAX_BUCKETS_FACTOR];
    size_t          zKeyIndexListOffset;
    size_t          zmmpoolOffset;
    char            szMagic[32];
};
#define HashTableHeadSize       ((sizeof(HashTableHead)+ HashTableAlianSize -1 )/HashTableAlianSize*HashTableAlianSize)
#pragma pack()


//
//iMaxEntryNum
//f(iMaxEntryNum,1)
//..
//f(iMaxEntryNum,iMaxBucketsFactor)
//iMaxEntryNum

//size = head + sizeof(int)*(f(iMaxEntryNum,1...iMaxBucketsFactor)+iMaxEntryNum)+mmpool


//a hash table flat in a memory block
int     HashTable::Init(void* pBuffer,size_t zBufferSize,
             int iEntySize,int iMaxEntryNum,
             PFNHashCode hash,
             PFNHashCompare comp,
             int iMaxBucketsFactor )
{
    return -1;
}
/////////////////////////////////////////////////////////////////////////////////////////////////
int     HashTable::Attach(void* pBuffer,size_t zBufferSize,
             int iEntySize,int iMaxEntryNum,
             PFNHashCode hash,
             PFNHashCompare comp,
             int iMaxBucketsFactor )
{
    return -1;
}
size_t  HashTable::GetBufferSize(int iEntySize,int iMaxEntryNum,int iMaxBucketsFactor )
{
    size_t sz = 0;
    if(iMaxBucketsFactor < 1 || iMaxBucketsFactor > DEFAULT_MAX_BUCKETS_FACTOR)
    {
        return 0;
    }
    OnTheFlyPrimeTable     ptble(iMaxEntryNum);    
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
    sz += iMaxEntryNum;
    sz *= sizeof(int);
    sz += HashTableHead;
    sz += MemPool::GetBufferSize(iEntySize,iMaxEntryNum);    
    return sz;
}
int     HashTable::Insert(void* pEntry)
{
    //
    return -1;
}
int     HashTable::Remove(void* pEntry)
{
    return -1;
}
void*   HashTable::Find(void* pEntry)
{
    return -1;
}
//////////////////////////
int     HashTable::MaxCount()
{
    return -1;
}
int     HashTable::Count()
{
    return -1;
}
//head is 0
void*   HashTable::GetNextEntry(int & itr)
{
    return -1;
}
int     HashTable::Check()
{
    return -1;
}
    
