#include "Log.h"
#include "stdinc.h"
#include "DateTime.h"    

Log::Log(LogLevel iLv)
{
    iFilterLv = iLv;     
    //pszBuffer = NULL;
}
Log::~Log()
{
    if(ptrFile)
    {
        ptrFile->Close();
        ptrFile.reset();
    }
}

int Log::Init(const char* pszLogFileName,LogLevel iFilterLv_,int iSingleFileKBSize)
{
	iFilterLv = iFilterLv_;
    if(pszLogFileName != NULL)
    {
        ptrFile.reset(new LogFile(pszLogFileName,0,iSingleFileKBSize*1024));
    }    
    LOG_INFO("Log initialize min filter lv = %d ",iFilterLv);
    return 0;
}
int Log::Write(LogLevel lv,const char* pszFile,const char* pszFunction,int line,const char* szfmt, ...)
{	
    static const char * pszLogLv[] = {"LOG_LV_0","TRACE","DEBUG","INFO","WARN","ERROR","FATAL"};
    if(lv < iFilterLv ||
       lv > (int)(sizeof(pszLogLv)/sizeof(pszLogLv[0]) ))
    {
        return -1;
    }
    static char szBuffer[MAX_SZ_BUFFER_SIZE];// = new char[MAX_SZ_BUFFER_SIZE];
    const char * pszLineHeadFmt = "%s | %s | %s:%d | %s | ";
    snprintf(szBuffer,MAX_SZ_BUFFER_SIZE,pszLineHeadFmt,
            Time::now().ToString().c_str(),
			pszLogLv[lv],
            pszFile,line,pszFunction);
    int iLineLen = strlen(szBuffer);

    va_list marker;
    va_start( marker, szfmt );
    vsnprintf(szBuffer + iLineLen,
            MAX_SZ_BUFFER_SIZE - iLineLen,
            szfmt,marker);
    va_end(marker);

    iLineLen = strlen(szBuffer);
    if(iLineLen >= MAX_SZ_BUFFER_SIZE)
    {
        szBuffer[MAX_SZ_BUFFER_SIZE - 1] = '\0';
        iLineLen = MAX_SZ_BUFFER_SIZE - 1;
    }
    
    if(ptrFile)
    {
        ptrFile->Write(szBuffer,iLineLen); 
        ptrFile->Write("\r\n",2);
    }
    else
    {
        fprintf(stderr,"%s",szBuffer);
        fprintf(stderr,"%s","\r\n");
    }
	return 0;
}





