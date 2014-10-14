#pragma once
#include "base/stdinc.h"

class MemPool
{
public:
    int     Init(void* pBuffer,size_t zBufferSize,
                 int iEntySize,int iMaxEntryNum);
    /////////////////////////////////////////////////////////////////////////////////////////////////
    int     Attach(void* pBuffer,size_t zBufferSize,
                 int iEntySize,int iMaxEntryNum);
    
    static  size_t  GetBufferSize(int iEntySize,int iMaxEntryNum);
    //return idx
    int     Alloc();
    int     Free(int idx);
    //idx
    int     Addr2Idx(void* p);
    int     Idx2Addr(int idx);
    void*   operator [](int idx);
    //////////////////////////
    int     MaxCount();
    int     Count();    
    //head is 0
    void*   GetNextEntry(int & itr);    
protected:
    int     Check();
public:
    void*    m_pBufferBase;
    size_t   m_zBuffSize;
    int      iEntrySize;
};

