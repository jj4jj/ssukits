
#include "MFileMap.h"

#if 1
MFileMap::MFileMap():fd(0)
{

} 
MFileMap::~MFileMap()
{
    Destroy();
}
#endif


#if 1
int MFileMap::Init(const char* pszFile,int iFlags ,int iProt ,int iType  )
{
    if( pBaseAddr == NULL)
    {
        LOG_FATAL("init pBaseAddr is null");
        return -1;
    }
    int fd = open(pszFile,O_RDWR);
    if(fd < 0)
    {
        LOG_ERROR("open file = %s is error no = %d",pszFile,errno);
        return -1;
    }
    zLen = 0;
    pBaseAddr = MapFile(fd,zLen,0,iFlags,iProt,iType);
    close(fd);
    if(NULL == pBaseAddr)
    {
        return -1;
    }
    return 0;
}
void MFileMap::Destroy()
{
    DestroyMap(pBaseAddr,zLen);  
    pBaseAddr = NULL;
    zLen = 0;
}
void    MFileMap::SetLength(size_t sz)
{
    zLen = sz;
}
int MFileMap::Sync()
{
    return Sync(pBaseAddr,zLen);
}

void* MFileMap::MapFile(int fd,size_t & len,size_t offset,int iFlags ,int iProt,int iType )
{
    if(0 == len)
    {
        struct stat sb;
        if (fstat(fd, &sb) < 0) 
        {  
            LOG_ERROR("fstate error no = %d",errno);
            return NULL;
        }          
        len = sb.st_size;
    }
    void * p = mmap(NULL,len,iProt,iType,fd,offset);
    if(pBaseAddr == (void*)-1)
    {
        LOG_ERROR("mmap error no = %d",errno);
        return NULL;
    }
    
    return p;
}
int MFileMap::DestroyMap(void* p,size_t len)
{
    if(p && munmap(p,len) < 0) 
    {  
        LOG_ERROR("unmap file is error no = %d !",errno);
        return -1;
    } 
    return 0;
}
int MFileMap::Sync(void* p,size_t zLen,int iFlags)
{
    if(msync(p,zLen,iFlags) < 0)
    {
        LOG_ERROR("msync error no = %d",errno);
        return -1;
    }
    return 0;
}
#endif

