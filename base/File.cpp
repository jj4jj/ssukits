#include "CommonMacro.h"
#include "File.h"
#include "Log.h"


static int FileAttrCompByLastModifyTime(struct stat & s1,struct stat& s2)
{
    return (s1.st_mtime - s2.st_mtime);
}
static int FileAttrCompBySize(struct stat & s1,struct stat & s2)
{
    return (s1.st_size - s2.st_size);
}
File::FileAttrCompare File::sFileSortByAttr[MAX_SUPPORT_ATTR]=
{
    FileAttrCompByLastModifyTime,
    FileAttrCompBySize
};


#if 1
void File::Construct()
{
    pFile = NULL;
    szFilePath[0] = '.';
    szFilePath[1] = '\0';
}
void File::Destruct()
{
    szFilePath[0] = '.';
    szFilePath[1] = '\0';
}
File::File()
{
    Construct();
}
File::File(const char * pszFileName,const char* pszMode)
{
    Construct();       
    Open(pszFileName,pszMode);
}

File::~File()
{
    Destruct();
}
#endif

#if 1
int File::Open(const char*  pszFileName,const char* pszMode)
{    
    pFile = fopen(pszFileName,pszMode);
    if(!pFile)
    {
        LOG_ERROR("fopen error errno = %d for = %s",errno,strerror(errno));
        return -1;
    }
    snprintf(szFilePath,sizeof(szFilePath),"%s",pszFileName);
    return 0;
}
void File::Close()
{
    if(pFile)
    {
        fclose(pFile);
        pFile = NULL;
    }
    Destruct();
}
long File::GetFileSize(const char* pszFilePath)
{
	struct stat statbuff;
	if(stat(pszFilePath, &statbuff) < 0)
     {
        return 0;
	}
	return statbuff.st_size;
}
const char* File::GetPathDirName(const char* pszFilePath,char* pszDirNameBuff,int iBuffLen)
{    
    int len = strlen(pszFilePath);
    int iDirPathLen = 0;
    for(int i = len ; i >= 0;  --i)
    {
        if(pszFilePath[i] == DELIMS)
        {
            iDirPathLen = i;
            break;
        }
    }
    if(iDirPathLen > 0 && iDirPathLen < iBuffLen)
    {
        memcpy(pszDirNameBuff,pszFilePath,iDirPathLen);
        pszDirNameBuff[iDirPathLen] = '\0';
        return pszDirNameBuff;
    }
    //current dir
    STRNCPY(pszDirNameBuff,iBuffLen,".");
    return pszDirNameBuff;
}
const char* File::GetPathBaseName(const char* pszFilePath,char* pszBaseNameBuff,int iBuffLen)
{
    int len = strlen(pszFilePath);
    int i = len;
    if(0 == len )
    {
        return NULL;
    }
    for(; i >= 0;  --i)
    {
        if(pszFilePath[i] == DELIMS)
        {
            break;
        }
    }
    if(i < 0)
    {
        i = 0;
    }
    else
    {
        ++i;
    }
    //len - i
    if(len < iBuffLen + i)
    {
        memcpy(pszBaseNameBuff,pszFilePath + i, len + 1 - i);
        return pszBaseNameBuff;
    }
    return NULL;
}

bool File::Exist(const char * pszFile)
{
    return (access(pszFile, F_OK) == 0);
}
int File::Rename(const char* pszOld,const char* pszNew)
{
    return rename(pszOld,pszNew);  
}
void  File::FileListSort(std::vector<string> &  files,const char* pszDir,FileAttrCompare pfnCmp)
{
    //create index , compare 
    typedef std::map<string,struct stat>          FileStatMap;
    //typedef FileStatMap::iterator                 FileStatMapItr;
    
    FileStatMap   mp;
    string sFilePath = "";
    if(pszDir != NULL)
    {
        sFilePath += pszDir;
        sFilePath += DELIMS;
    }
    string sFilePathName ;
    struct  stat statbuff;    
    for(int i = 0; i < (int)files.size() ; ++i)
    {
        sFilePathName = sFilePath + files.at(i);   
    	if( stat(sFilePathName.c_str(), &statbuff ) < 0)
        {
            bzero(&statbuff,sizeof(statbuff));
    	}
        mp[files.at(i)] = statbuff;
    }
    
    //------------------------------------------------------------    
    //sorted
    for(int i = 1 ;i < (int)files.size(); ++i)
    {
        //insert i-th element to front sorted list.
        for(int j = i; j > 0; --j)
        {
            if(pfnCmp(mp[files[j]],mp[files[j-1]]) < 0)
            {
                files[j-1].swap(files[j]);
            }
            else
            {
                break;
            }
        }
    }
    
}

int  File::ListFiles(std::vector<std::string> &  files,const char* pszDir)
{
	DIR *dfd = NULL;
	struct dirent *dp = NULL;
    files.clear();
	if ((dfd = opendir(pszDir)) == NULL)
    {
        printf("open dir = %s error !",pszDir);
		return -1;
	}
	while ((dp = readdir(dfd)) != NULL)
    {
		if (strcmp(dp->d_name, ".") == 0
		    || strcmp(dp->d_name, "..") == 0)
		{
			continue;    /* skip self and parent */
        }
		if (strlen(dp->d_name) > MAX_FILE_NAME_LEN)
		{
            printf("error too long ( > %d )name = %s ",MAX_FILE_NAME_LEN,dp->d_name);   
            continue;
        }
		else 
        {
            files.push_back(std::string(dp->d_name));
		}
	}
	closedir(dfd);
    return 0;
}
int    File::Flush()
{
    return fflush(pFile);
}
int     File::WriteFormat(const char* pszFormat,...)
{
	va_list ap;
	va_start(ap, pszFormat);
    int ret = vfprintf(pFile,pszFormat,ap);
	va_end(ap);
    return ret;
}

int     File::ScanFormat(const char* pszFormat,...)
{
	va_list ap;
	va_start(ap, pszFormat);
    int ret = vfscanf(pFile,pszFormat,ap);
	va_end(ap);
    return ret;
}
int    File::Seek(int whence,int64_t offset )
{
    return    fseek(pFile,offset,whence);
}

const char*  File::GetLine(char* pBuffer,int iLen)
{
    return fgets(pBuffer,iLen,pFile);
}
int File::Read(char * pBuffer,int iLen)
{
    return   fread(pBuffer,iLen,1,pFile) != 1 ? -1 : 0;
}
int File::Write(const char* pBuffer,int iLen)
{
    return fwrite(pBuffer,iLen,1,pFile)!=1 ? -1 : 0;
}
int    File::ShareLock()
{
    int fd = GetFD();
    if(fd >= 0)
    {
        return flock(fd,LOCK_SH);
    }
    return -1;
}
int    File::ExcluLock()
{
    int fd = GetFD();
    if(fd >= 0)
    {
        return flock(fd,LOCK_EX);
    }
    return -1;
}
void   File::Unlock()
{
    int fd = GetFD();
    if(fd >= 0)
    {
        flock(fd,LOCK_UN); 
    }
}
int   File::GetFD()
{
    if(pFile)
    {
        return  fileno(pFile);
    }
    return -1;
}
long File::GetCurrentFileSize()
{
    long curpos, length;
    curpos = ftell(pFile); 
    fseek(pFile, 0L, SEEK_END);  
    length = ftell(pFile);
    fseek(pFile, curpos, SEEK_SET);    
    return length;
}
long File::GetAvailBytes()
{
    long curpos = 0;
    curpos = ftell(pFile); 
    long length = 0;
    fseek(pFile, 0L, SEEK_END);  
    length = ftell(pFile);
    fseek(pFile, curpos, SEEK_SET);    
    return (length - curpos);    
}


#endif

