#pragma once


class ShareMemory
{
public:    
    int     Attach(int iKey, size_t size);
    //if not exist , create it , if exist attach it
    int      Create(int iKey, size_t size, int iFlag );
    void     Detach();
    void    Destroy();
    void*   GetData();
public:
    static  int     PathToKey(const char *pszPath,int iID);
    static  int     Get(int iKey,size_t sz,int iCreateFlag,int iIOFlag = 0,void * pAssignBase = NULL);
    static  void    Destroy(int shmid);
    static  int     Control(int shmid, int cmd, struct shmid_ds *buf);
public:
    ShareMemory();
    ~ShareMemory();
private:
    int         iShmId;
    void    *   pDataBase;
};



