#pragma once

#include "File.h"

class LogFile : public File
{
public:
    enum
    {
        //10MB
        DEFAULT_MAX_LOG_SINGLE_FILE_SIZE = 10*1024*1024,
        DEFAULT_MAX_LOG_HISTROY_FILE_NUM = 10,
    };
public:
    void    Construct();
    void    Destruct();  
    LogFile(const char*  pszPrefix,
            int iMaxLogFiles = DEFAULT_MAX_LOG_HISTROY_FILE_NUM,
            long lMaxLogSingleFileSize = DEFAULT_MAX_LOG_SINGLE_FILE_SIZE);
    virtual ~LogFile();    
public:
    //return 
    int      Open(const char * pszPrefix, int _iMaxLogFiles , long _lMaxLogSingleFileSize ,bool bNewLogFile = false );    
    int      Write(const char* pBuffer,int iLen);
    int      GetLastLogFileOrder();
    int      ShiftNext();
    string   GetLogFileName(int iOrder);
private:
    long        lMaxLogSingleFileSize;
    int         iMaxHistoryLogFileNum;
    char        szFilePrefix[256];
    int         iLastLogFileOrder;
};

