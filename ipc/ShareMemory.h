#pragma once


class ShareMemory
{
public:    
    int     Attach(int iKey, size_t size)
    {
        //if not exist ,return error
        return Get(iKey,size,IPC_EXCL);
    }
    //if not exist , create it , if exist attach it
    int      Create(int iKey, size_t size, int iFlag )
    {
        int iPrivFlag = iFlag&(~IPC_EXCL);
        return Get(iKey,size,iPrivFlag|IPC_CREAT);
    }
    void     Detach()
    {
        if(iShmId >= 0)
        {
            if(pDataBase)
            {
                if(shmdt(pDataBase) < 0)
                {
                    LOG_ERROR("detach shm = %p error = %d",errno);
                }
            }
        }
        iShmId = -1;
        pDataBase = NULL;
        return 0;        
    }
    void    Destroy()
    {
        Destroy(iShmId);
    }
    void*   GetData()
    {
        return pDataBase;
    }
public:
    static  int     PathToKey(const char *pszPath,int iID)
    {
         return ftok(pszPath,iID);
    }
    static  int     Get(int iKey,size_t sz,int iCreateFlag,int iIOFlag = 0,void * pAssignBase = NULL)
    {
        int shmid = shmget(iKey,sz,iFlag);
        if ( shmid < 0 )
        {
            LOG_ERROR("shmget error no = %d",errno);
            return -1 ;
        }
        
        void * shmaddr = shmat( shmid,pAssignBase,iIOFlag) ;        
        if ( (int)shmaddr == -1 )
        {
            LOG_ERROR("shmat addr error no = %d",errno) ;
            //- need to dt ? no 
            return -1 ;
        }   
        iShmId = shmid;
        pDataBase = shmaddr;
        return 0;
    }
    static  void    Destroy(int shmid)
    {
        if( shmctl(shmid,IPC_RMID,NULL) < 0)
        {
            LOG_ERROR("remove shm id = %d error = %d",shmid);
        }
    }
    static  int     Control(int shmid, int cmd, struct shmid_ds *buf)
    {
        int ret = shmctl(shmid, cmd,buf);
        if(ret < 0)
        {
            LOG_ERROR("control shm id = %d cmd = %d error = %d",shmid);
        }
        return ret;
    }
public:
    ShareMemory():iShmID(-1),pDataBase(NULL){}
    ~ShareMemory()
    {
        Detach();
    }
private:
    int         iShmId;
    void    *   pDataBase;
};



