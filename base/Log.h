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
 *   Last Changed: 05/22/2014 08时30分33秒
 *
 * =====================================================================================
 */
#pragma once
#include "stdinclude.h"
#include "Singleton.hpp"

class CLog : Singleton<CLog>
{
public:
	Init(int iFilterLv_ = 0){iFilterLv = iFilterLv;}
	Write(int lv,const char* szFile,int line,const char* szfmt, ...);
private:
	int iFilterLv;
};

#define LOG(lvl, fmt, args...)	 do{\
CLog::Instance().Write(lvl, __FILE__, __FUNCTION__, __LINE__ , fmt, ##args);\
}while(false)

#define LOG_TRACE(fmt,args...) LOG(1,fmt,##args)
#define LOG_DEBUG(fmt,args...) LOG(2,fmt,##args)
#define LOG_DEBUG(fmt,args...) LOG(3,fmt,##args)
#define LOG_WARN(fmt,args...)  LOG(4,fmt,##args)
#define LOG_ERROR(fmt,args...) LOG(5,fmt,##args)
#define LOG_FATAL(fmt,args...) LOG(6,fmt,##args)
