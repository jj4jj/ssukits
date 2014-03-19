
#pragma once
#include "base/stdinc.h"

class MFileMap
{
public:
    int     Init(const char* pszFile,int iFlags = O_RDWR ,int iProt = PROT_READ|PROT_WRITE,int iType = MAP_SHARED );
    void    Destroy();
    void    SetLength(size_t sz);
    int     Sync();
public:
    static void* MapFile(int fd,size_t & len,size_t offset = 0,int iFlags = O_RDWR ,int iProt = PROT_READ|PROT_WRITE,int iType = MAP_SHARED);
    static int DestroyMap(void* p,size_t len);
    static int Sync(void* p,size_t zLen,int iFlags = MS_SYNC);   
public:
    MFileMap();
    ~MFileMap();
private:
    void* pBaseAddr;
    size_t zLen;
};


