#pragma once

#include "base/stdinc.h"


typedef size_t  (*PFNHashCode) (void* v);
typedef int  (*PFNHashCompare) (void* v1,void* v2);

//a hash table flat in a memory block
class HashTable
{
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
    void*   GetNextEntry(int & itr);    
protected:
    int     Check();
public:
    void*    m_pBuffer;
    size_t   m_zBuffSize;
    PFNHashCode hash;
    PFNHashCompare comp;
    ///////////////////////////
    int         iMaxEntryNum;
    int         iMaxBucketsFactor;
    //////////////////////////////
    MemPool     mempool;       
};

