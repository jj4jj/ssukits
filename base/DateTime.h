
#pragma once

#include "stdinc.h"

class Time
{

public:
    static Time now(struct timezone *tz = NULL);    
    static void now(struct timeval & tv,struct timezone *tz = NULL);
    static int64_t uspast(struct timeval & after,struct timeval & before);
    static void usappend(struct timeval & time , int64_t append);
    static time_t mkTimeStamp(const char* pszTime,const char* pszFormat = NULL);
    static time_t GetTimeStampNow();
    //todo
    //static const char* fmtTime(time_t t,const char* pszFormat,char* buffer,int bflen);
public:
    Time(time_t tm = 0);    
    Time(struct timeval _tv);
    Time(struct timeval _tv,struct timezone);
    string ToString();
    char* ToString(char* pszBuffer,int iBuffLen);        
public:
    struct timeval tv;
    struct timezone tz;
};


class DateTime
{
public:
    //todo
};


