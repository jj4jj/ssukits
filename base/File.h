
#pragma once
#include "stdinc.h"


class File
{
public:
    enum
    {
        //100MB
        DEFAULT_MAX_LOG_SINGLE_FILE_SIZE = 100*1024*1024,
        MAX_FILE_NAME_LEN = 256,
    };
public:
    void    Construct();
    void    Destruct();
    File();
    File(const char*  pszFilePath,const char* pszMode);
    virtual ~File();
public:
    int  Open(const char*  pszFilePath,const char* pszMode);
    void Close();
    long GetFileSize();
    long GetAvailBytes();        
    //return 
    virtual int  Write(const char* pBuffer,int iLen);
    virtual int  Read(char * pBuffer,int iLen);
public:
    static bool Exist(const char* pszFile);
    static int  Rename(const char* pszOld,const char* pszNew);
    static int  ListFiles(std::vector<string> &  files,const char* pszDir);
private:
    FILE* pFile;
    //char        szFilePath[256];    
};


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
    int      Open(const char * pszPrefix, int _iMaxLogFiles , long _lMaxLogSingleFileSize );    
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






