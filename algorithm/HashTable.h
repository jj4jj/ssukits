#pragma once

#include "MemPool.h"

/////////////////////////////////////////////////

typedef size_t  (*PFNHashCode) (void* v);
typedef int  (*PFNHashCompare) (void* v1,void* v2);

//a hash table flat in a memory block
class HashTable
{
public:
    enum
    {
        DEFAULT_MAX_BUCKETS_FACTOR  =   7,
    };
public:
    int     Init(void* pBuffer,size_t zBufferSize,
                 int iEntySize,int iMaxEntryNum,
                 PFNHashCode hash,
                 PFNHashCompare comp,
                 int iMaxBucketsFactor = 3);
    /////////////////////////////////////////////////////////////////////////////////////////////////
    int     Attach(void* pBuffer,size_t zBufferSize,
                 int iEntySize,int iMaxEntryNum,
                 PFNHashCode hash,
                 PFNHashCompare comp,
                 int iMaxBucketsFactor = 3);    
    static  size_t  GetBufferSize(int iEntySize,int iMaxEntryNum,int iMaxBucketsFactor = 3);
    int     Insert(void* pEntry);                 
    int     Remove(void* pEntry);                 
    void*   Find(void* pEntry);
    //////////////////////////
    int     MaxCount();
    int     Count();    
    //head is 0
    int     GetBeinEntry();
    void*   GetNextEntry(int & itr);    
    static  int     Check(void* pBuffer,int iEntrySize,int iMaxEntryNum,int iMaxFactor);
public:
    void*          m_pBuffer;
    PFNHashCode    m_fnHash;
    PFNHashCompare m_fnComp;
    //////////////////////////////
    MemPool        mempool;       
};

