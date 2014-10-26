
#include "LogFile.h"

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
    Construct();
    Open(pszPrefix,_iMaxLogFiles,_lMaxLogSingleFileSize);
}
LogFile::~LogFile()
{    
    Destruct();
}

int LogFile::Open(const char * pszPrefix, int _iMaxLogFiles , long _lMaxLogSingleFileSize , bool bNew )
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
    ////////////////////////////////////////////////////
    int ret = 0;
    if(bNew)
    {
        ret = File::Open(GetLogFileName(GetLastLogFileOrder()).c_str(),"w+");
    }
    else
    {
        ret = File::Open(GetLogFileName(GetLastLogFileOrder()).c_str(),"a+");
    }        
    return ret;
}

int      LogFile::GetLastLogFileOrder()
{
    //time sorted list . last one
    //get its order
    if(iLastLogFileOrder == 0)
    {
        //1 //todo
        //
        vector < std :: string > files;
        char szFilePathDirName[256];
        GetPathDirName(szFilePath,szFilePathDirName, sizeof(szFilePath));
        ListFiles(files,szFilePathDirName);
        FileListSort(files,szFilePathDirName,sFileSortByAttr[FILE_ATTR_LAST_MODIFY_TIME]);
        for(int i = (int)files.size() - 1; i >= 0; --i)
        {
            string & sLatestFileName = files[i];
            int iPos = sLatestFileName.rfind(szFilePrefix);
            if(iPos == (int)string::npos)
            {
                continue;
            }
            //1 for delims (.)
            const char* pszID = sLatestFileName.c_str() + iPos + strlen(szFilePrefix) + 1;
            int id = 1;
            if(sscanf(pszID,"%d",&id) == 1)
            {
                iLastLogFileOrder = id;
            }          
            return id;
        } 
        iLastLogFileOrder = 1;
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
    //format
    snprintf(szBuffer,sizeof(szBuffer),
                "%s.%03d",
                szFilePrefix,iOrder);
    return string(szBuffer);
}
int LogFile::ShiftNext()
{
    File::Close();
    //--
    iLastLogFileOrder = (iLastLogFileOrder + 1) % ( iMaxHistoryLogFileNum + 1);
    return    Open(szFilePrefix,0,0,true);
}
int LogFile::Write(const char* pBuffer,int iLen)
{
    if(GetCurrentFileSize() + iLen > lMaxLogSingleFileSize)
    {
        ShiftNext();
    }    
    return File::Write(pBuffer,iLen);
}


#endif

