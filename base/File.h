
#pragma once
#include "stdinc.h"


class File
{
public:
    //unix file path delims
    #define     DELIMS      ('/')
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
    inline FILE*  GetFilePointer(){return pFile;}
public:
    int  Open(const char*  pszFilePath,const char* pszMode);
    void Close();
    long GetCurrentFileSize();
    long GetAvailBytes();        
    //return 
    virtual int  Write(const char* pBuffer,int iLen);
    virtual int  Read(char * pBuffer,int iLen);
    int     ShareLock();
    int     ExcluLock();
    void    Unlock();
    int     GetFD();
    const char*  GetLine(char* pBuffer,int iLen);
public:
    static bool Exist(const char* pszFile);
    static int  Rename(const char* pszOld,const char* pszNew);
    static int  ListFiles(std::vector<string> &  files,const char* pszDir);
    static long GetFileSize(const char* pszFilePath);
    static const char* GetPathDirName(const char* pszFilePath,char* pszBaseNameBuff,int iBuffLen);
    static const char* GetPathBaseName(const char* pszFilePath,char* pszBaseNameBuff,int iBuffLen);

    //1,0,-1
    typedef int (*FileAttrCompare)(struct stat & pFileStat1,struct stat & pFileStat2);
    static void  FileListSort(std::vector<string> &  files,const char* pszDir,FileAttrCompare pfnCmp);
protected:
    FILE* pFile;
    char        szFilePath[256]; 
public:
    enum 
    {
        FILE_ATTR_LAST_MODIFY_TIME = 0,
        FILE_ATTR_SIZE = 1,            
        MAX_SUPPORT_ATTR,
    };
    static  FileAttrCompare sFileSortByAttr[MAX_SUPPORT_ATTR];
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






