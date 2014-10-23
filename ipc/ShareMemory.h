#pragma once

#include "base/stdinc.h"
class ShareMemory
{
public:
    #define DEFAULT_ACCESS  (0664)
public:    
    bool    Exists(int iKey,int iFlag = DEFAULT_ACCESS);
    int     Attach(int iKey,size_t size, int iFlag = DEFAULT_ACCESS);
    int     GetAttachNum();
    //if not exist , create it , if exist attach it
    int     Create(int iKey, size_t size, int iFlag = DEFAULT_ACCESS);
    void    Detach();
    void    Destroy();
    void*   GetData();
    int     GetShmID();
public:
    static  int     PathToKey(const char *pszPath,int iID = 1);
    static  void    Destroy(int shmid);
    static  int     Control(int shmid, int cmd, struct shmid_ds *buf);
protected:
    int     AuxGet(int iKey,size_t sz,int iCreateFlag,int iIOFlag = 0,void * pAssignBase = NULL);
public:
    ShareMemory();
    ~ShareMemory();
private:
    int         iShmId;
    void    *   pDataBase;
};



