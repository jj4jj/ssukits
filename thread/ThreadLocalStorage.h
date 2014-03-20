#pragma once

#include "base/stdinc.h"


//must be in global life circle
class ThreadLoacalStorage
{
public:
    typedef void (*FNDestroyValue)(void* p);    
    ThreadLocalStorage();
public:
    int     Init(FNDestroyValue _fnDestroyValue);
    void*    GetLocalValue();
    int      SetLocalValue(const void* val);
    static int CreateKey();

private:
    pthread_key_t    key;
    pthread_once_t   create_key_once;
    FNDestroyValue   fnDestroyValue;
};

