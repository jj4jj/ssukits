
#include "ShareMemory.h"
#include "base/Log.h"


#if 1
ShareMemory::ShareMemory():iShmId(-1),pDataBase(NULL)
{

}
ShareMemory::~ShareMemory()
{
    //Detach();
}
#endif



#if 1
bool    ShareMemory::Exists(int iKey,int iFlag)
{    
    int mshmid = shmget(iKey,0,IPC_CREAT|IPC_EXCL|iFlag);
    if ( mshmid < 0 && errno == EEXIST )
    {
        LOG_DEBUG("shm key = %d is already exist .",iKey);
        return true;
    }    
    else
    {
        LOG_DEBUG("get old shm id = %d errno = %d .",
                  mshmid,errno);
        if(mshmid >= 0)
        {
            Destroy(mshmid);
        }
        return false;
    }
}
int     ShareMemory::GetAttachNum()
{
    //todo
    struct shmid_ds buf ;    
    int    flag = shmctl(iShmId, IPC_STAT, &buf) ;
    if ( flag == -1 )
    {
        LOG_ERROR("shmctl shm error") ;
        return -1;
    }
    return buf.shm_nattch;
}

int     ShareMemory::Attach(int iKey,size_t size, int iFlag)
{
    if(!Exists(iKey,iFlag))
    {
        LOG_ERROR("attach an not exist memory ! key = %d",iKey);
        return 1;
    }
    //if not exist ,return error .
    iFlag &=  (~IPC_CREAT);
    return AuxGet(iKey,size,iFlag);
}
//if not exist , create it , if exist attach it
int      ShareMemory::Create(int iKey, size_t size, int iFlag )
{
    //if already exists, fail
    iFlag |= IPC_CREAT;    
    iFlag |= IPC_EXCL;
    return AuxGet(iKey,size,iFlag);
}
void    ShareMemory:: Detach()
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
}
void    ShareMemory::Destroy()
{
    Destroy(iShmId);
}
void*   ShareMemory::GetData()
{
    return pDataBase;
}
int     ShareMemory::GetShmID()
{
    return iShmId;
}

#endif

#if 1
int     ShareMemory::PathToKey(const char *pszPath,int iID)
{
     return ftok(pszPath,iID);
}
void    ShareMemory::Destroy(int shmid)
{
    if( shmctl(shmid,IPC_RMID,NULL) < 0)
    {
        LOG_ERROR("remove shm id = %d error = %d",shmid);
    }
}
int     ShareMemory::Control(int shmid, int cmd, struct shmid_ds *buf)
{
    int ret = shmctl(shmid, cmd,buf);
    if(ret < 0)
    {
        LOG_ERROR("control shm id = %d cmd = %d error = %d",shmid);
    }
    return ret;
}
#endif

#if 1
int     ShareMemory::AuxGet(int iKey,size_t sz,int iCreateFlag,int iIOFlag ,void * pAssignBase )
{
    int shmid = shmget(iKey,sz,iCreateFlag);
    if ( shmid < 0 )
    {
        LOG_ERROR("shmget error no = %d",errno);
        return -1 ;
    }    
    void * shmaddr = shmat( shmid,pAssignBase,iIOFlag);        
    if ( shmaddr == (void*)-1 )
    {
        LOG_ERROR("shmat addr error no = %d",errno) ;
        //- need to dt ? no 
        return -1 ;
    }   
    LOG_DEBUG("get shm id = %d size = %zu ok .",shmid,sz);
    iShmId = shmid;
    pDataBase = shmaddr;
    return 0;
}

#endif
