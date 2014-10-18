#include "DateTime.h"
#include "StringUtil.h"



#define US_S_FACTOR     (1000000)


#if 1

Time::Time(time_t tm)
{
    tv.tv_sec = 0;
    tv.tv_usec = 0;
    tz.tz_minuteswest = 0;
    tz.tz_minuteswest = 0;    
    //
    tv.tv_sec = tm;
}
Time::Time(struct timeval _tv)
{
    tv = _tv;
}
Time::Time(struct timeval _tv,struct timezone _tz)
{
    tv = _tv;
    tz = _tz;    
}


Time Time::now(struct timezone *tz )
{
    struct timeval tv = {0,0};
    if(gettimeofday(&tv,tz) != 0 )
    {
        printf("gettimeofday return error no = %d",errno);
    }
    
    if(NULL != tz)
    {
        return Time(tv,*tz);
    }
    else
    {
        return Time(tv);
    }
}
void Time::now(struct timeval & tv,struct timezone *tz )
{
    gettimeofday(&tv,tz);
}
int64_t Time::uspast(struct timeval & after,struct timeval & before)
{
    return (after.tv_sec-before.tv_sec)*US_S_FACTOR + after.tv_usec - before.tv_usec;
}
void Time::usappend(struct timeval & time , int64_t append)
{
    if(append > 0)
    {
        time.tv_sec += append/US_S_FACTOR;
        time.tv_usec += append%US_S_FACTOR;
        if(time.tv_usec > US_S_FACTOR)
        {
            time.tv_usec = time.tv_usec - US_S_FACTOR;
            ++time.tv_sec;
        }
    }
    else if(append < 0)
    {
        append = 0 - append;
        int us = append % US_S_FACTOR;
        int s = append / US_S_FACTOR;
        if(us > time.tv_usec)
        {
            --time.tv_sec;
            time.tv_usec = US_S_FACTOR - us + time.tv_usec;
        }
        else
        {
            time.tv_usec -= us;
        }
        if(time.tv_sec >= s)
        {
            time.tv_sec -= s;
        }
        else
        {
            time.tv_sec = 0;
            time.tv_usec = 0;
        }
    }
}
time_t Time::mkTimeStamp(const char* psTime,const char* pszFormat )
{
    string sdatetime = psTime;
    StringUtil::ReplaceAll(sdatetime," ","-");
    StringUtil::ReplaceAll(sdatetime,":","-");
    StringUtil::ReplaceAll(sdatetime,"/","-");

    char* pszTime = const_cast<char*>(sdatetime.c_str());
    //char sTemp[20] , s1[5] ;
    struct tm tmTime;
    time_t lTime ;

	int iNum = 0;
	int aiValues[6];
	iNum = StringUtil::SplitNum(pszTime, aiValues, 6, "-");
	if(iNum < 6)
	{
		return 0;
	}
	tmTime.tm_year = aiValues[0]-1900;
	tmTime.tm_mon  = aiValues[1]-1;
	tmTime.tm_mday = aiValues[2];
	tmTime.tm_hour = aiValues[3];
	tmTime.tm_min  = aiValues[4];
	tmTime.tm_sec  = aiValues[5];
	
    tmTime.tm_isdst=0; //standard time , not dst
    lTime =mktime(&tmTime);	
	return lTime ;
}
time_t Time::GetTimeStampNow()
{
    return time(NULL);
}

/*
const char* Time::fmtTime(time_t t,const char* pszFormat,char* buffer,int bflen)
{
    //todo
    
}
*/

string Time::ToString()
{
    char szBuffer[64];
    return string( ToString(szBuffer,(int)sizeof(szBuffer)) );
}
char* Time::ToString(char* pszBuffer,int iBuffLen)
{
	struct tm curr;
	if(localtime_r(&(tv.tv_sec),&curr) != NULL)
	{
		snprintf(pszBuffer,iBuffLen,
                    "%04d-%02d-%02d-%02d-%02d-%02d.%lu", 
					(int)curr.tm_year+1900, (int)curr.tm_mon+1, 
					(int)curr.tm_mday,
					(int)curr.tm_hour,
					(int)curr.tm_min,
					(int)curr.tm_sec,
					tv.tv_usec);        
    }
    else
    {
        printf("localtime_r return null errno = %d",errno);
    }        
    return pszBuffer;    
}

#endif



