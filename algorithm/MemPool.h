#pragma once
#include "base/stdinc.h"

class MemPool
{
public:
    MemPool(){m_pBufferBase = NULL;}
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
    void*   Idx2Addr(int idx);
    void*   operator [](int idx);
    //////////////////////////
    int     MaxCount();
    int     Count();    
    int     GetEntryBegin();
    void*   GetNextEntry(int & itr);    
protected:
    int     Check(void* pBuffer,int iEntrySize,int iMaxEntryNum);
public:
    void*    m_pBufferBase;
};

