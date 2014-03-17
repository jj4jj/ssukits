
#pragma once

#include "stdinc.h"

class Time
{

public:
    static Time now(struct timezone *tz = NULL);    
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


