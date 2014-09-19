
#include "ShareMemory.h"
#include "base/Log.h"


#if 1
ShareMemory::ShareMemory():iShmId(-1),pDataBase(NULL)
{

}
ShareMemory::~ShareMemory()
{
    Detach();
}
#endif



#if 1
int     ShareMemory::Attach(int iKey, size_t size)
{
    //if not exist ,return error
    return AuxGet(iKey,size,IPC_EXCL);
}
//if not exist , create it , if exist attach it
int      ShareMemory::Create(int iKey, size_t size, int iFlag )
{
    int iPrivFlag = iFlag&(~IPC_EXCL);
    return AuxGet(iKey,size,iPrivFlag|IPC_CREAT);
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
    iShmId = shmid;
    pDataBase = shmaddr;
    return 0;
}

#endif
