
#pragma once


class MFileMap
{
public:
    int Init(const char* pszFile,int iFlags = O_RDWR ,int iProt = PROT_READ|PROT_WRITE,int iType = MAP_SHARED )
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
    void Destroy()
    {
        DestroyMap(pBaseAddr,zLen);  
        pBaseAddr = NULL;
        zLen = 0;
    }
    void    SetLength(size_t sz)
    {
        zLen = sz;
    }
    int Sync()
    {
        return Sync(pBaseAddr,zLen);
    }
public:
    void* MapFile(int fd,size_t & len,size_t offset = 0,int iFlags = O_RDWR ,int iProt = PROT_READ|PROT_WRITE,int iType = MAP_SHARED)
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
    static int DestroyMap(void* p,size_t len)
    {
        if(p && munmap(p,len) < 0) 
        {  
            LOG_ERROR("unmap file is error no = %d !",errno);
            return -1;
        } 
        return 0;
    }
    static int Sync(void* p,size_t zLen,int iFlags = MS_SYNC)
    {
        if(msync(p,zLen,iFlags) < 0)
        {
            LOG_ERROR("msync error no = %d",errno);
            return -1;
        }
        return 0;
    }
   
public:
    MFileMap():fd(0){} 
    ~MFileMap()
    {
        Destroy();
    }
private:
    void* pBaseAddr;
    size_t zLen;
};


