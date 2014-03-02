
#include "File.h"

#if 1
void File::Construct()
{
    pFile = NULL;
    //szFilePath[0] = '\0';
}
void File::Destruct()
{
    if(pFile)
    {
        fclose(pFile);
        pFile = NULL;
    }
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

int File::Open(const char*  pszFileName,const char* pszMode)
{    
    pFile = fopen(pszFileName,pszMode);
    if(!pFile)
    {
        printf("fopen error errno = %d",errno);
        return -1;
    }
    return 0;
}
void File::Close()
{
    Destruct();
}

bool File::Exist(const char * pszFile)
{
    return (access(pszFile, F_OK) == 0);
}
int File::Rename(const char* pszOld,const char* pszNew)
{
    return rename(pszOld,pszNew);  
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

int File::Read(char * pBuffer,int iLen)
{
    return   fread(pBuffer,iLen,1,pFile) != 1 ? -1 : 0;
}
int File::Write(const char* pBuffer,int iLen)
{
    return fwrite(pBuffer,iLen,1,pFile)!=1 ? -1 : 0;
}
long File::GetFileSize()
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
    long curpos, length;
    curpos = ftell(pFile); 
    fseek(pFile, 0L, SEEK_END);  
    length = ftell(pFile);
    fseek(pFile, curpos, SEEK_SET);    
    return (length - curpos);    
}



///////////////////////////////////////////////////////////////////

#if 1



void    LogFile::Construct()
{
    //100MB
    lMaxLogSingleFileSize = DEFAULT_MAX_LOG_SINGLE_FILE_SIZE;    
    iMaxHistoryLogFileNum = DEFAULT_MAX_LOG_HISTROY_FILE_NUM;
    iLastLogFileOrder = 0;
}
void    LogFile::Destruct()
{
    File::Destruct();
    iLastLogFileOrder = 0;
    szFilePrefix[0] = '\0';
    
}

LogFile::LogFile(const char * pszPrefix, int _iMaxLogFiles , long _lMaxLogSingleFileSize )
{    
    Open(pszPrefix,_iMaxLogFiles,_lMaxLogSingleFileSize);
}
LogFile::~LogFile()
{    
    Destruct();
}

int LogFile::Open(const char * pszPrefix, int _iMaxLogFiles , long _lMaxLogSingleFileSize )
{
    strncpy(szFilePrefix,pszPrefix,sizeof(szFilePrefix));        
    if(0 != _lMaxLogSingleFileSize)
    {
        lMaxLogSingleFileSize = _lMaxLogSingleFileSize;
    }
    if(0 != _iMaxLogFiles)
    {
        iMaxHistoryLogFileNum = _iMaxLogFiles;
    }
    return File::Open(GetLogFileName(GetLastLogFileOrder()).c_str(),"a+");
}

int      LogFile::GetLastLogFileOrder()
{
    //time sorted list . last one
    //get its order
    if(iLastLogFileOrder == 0)
    {
        //1 //todo
        return 1;        
    }
        
    return iLastLogFileOrder;
}
string   LogFile::GetLogFileName(int iOrder)
{
    if(iOrder < 1)
    {
        iOrder = 1;
    }
    char szBuffer[256];
    snprintf(szBuffer,sizeof(szBuffer),
                "%s.%03d",
                szFilePrefix,iOrder);
    return string(szBuffer);
}
int LogFile::ShiftNext()
{
    File::Close();
    //--
    iLastLogFileOrder = (iLastLogFileOrder + 1) % iMaxHistoryLogFileNum;
    return    Open(szFilePrefix,0,0);
}
int LogFile::Write(const char* pBuffer,int iLen)
{
    if(GetFileSize() + iLen > lMaxLogSingleFileSize)
    {
        ShiftNext();
    }    
    return File::Write(pBuffer,iLen);
}


#endif
