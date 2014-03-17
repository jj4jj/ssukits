#include "DateTime.h"

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



