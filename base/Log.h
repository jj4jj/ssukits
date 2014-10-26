/*
 * =====================================================================================
 *
 *       Filename:  Log.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2014年05月22日 07时46分09秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *   Last Changed: 05/26/2014 07时46分53秒
 *
 * =====================================================================================
 */
#pragma once

#include "Singleton.hpp"
#include "LogFile.h"

class Log : public Singleton<Log>
{
public:    
    enum LogLevel
    {
        LOG_LV_NO_LIMIT = 0,
        LOG_LV_TRACE = 1,
        LOG_LV_DEBUG = 2,
        LOG_LV_INFO =  3,
        LOG_LV_WARN =  4,
        LOG_LV_ERROR = 5,
        LOG_LV_FATAL = 6,
    };
public:
	int Init(const char* pszLogFileName = NULL,LogLevel iFilterLv_ = LOG_LV_DEBUG,int iSingleFileKBSize = 1024,int iMaxLogFileNum = 20);
	int Write(LogLevel lv,const char* pszFile,const char* pszFunction,int line,const char* szfmt, ...);
public:
	Log();
    ~Log();    
private:
	int iFilterLv;
    shared_ptr<File>    ptrFile;
    //char*       pszBuffer;
    enum
    {
        MAX_SZ_BUFFER_SIZE = 1024*1024,
    };
};



/////////////////////////////////////////////////////////////////

#define LOG_real(lvl, fmt, args...)	 do{\
Log::Instance().Write(lvl, __FILE__, __FUNCTION__, __LINE__ , fmt, ##args);\
}while(false)

#define LOG_TRACE(fmt,args...) LOG_real(Log::LOG_LV_TRACE,fmt,##args)
#define LOG_DEBUG(fmt,args...) LOG_real(Log::LOG_LV_DEBUG,fmt,##args)
#define LOG_INFO(fmt,args...)  LOG_real(Log::LOG_LV_INFO,fmt,##args)
#define LOG_WARN(fmt,args...)  LOG_real(Log::LOG_LV_WARN,fmt,##args)
#define LOG_ERROR(fmt,args...) LOG_real(Log::LOG_LV_ERROR,fmt,##args)
#define LOG_FATAL(fmt,args...) LOG_real(Log::LOG_LV_FATAL,fmt,##args)




