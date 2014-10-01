#pragma once

#include "base/stdinc.h"
#include "base/Singleton.hpp"
#include "ThreadSync.h"


//pthread_once
    //pthread_once_t
    //once
class ThreadOnceExcute
{
public:
    typedef void (*FNOnceExcuteRoutine)();
    int Init(FNOnceExcuteRoutine routine)
    {
        once = PTHREAD_ONCE_INIT;
        function = routine;
        return 0;
    }
    int Excute()
    {
        return pthread_once(&once,function);
    }
private:
    pthread_once_t  once;
    FNOnceExcuteRoutine function;
};

///////////////////////////////////////////////////////////////////
class ThreadKeyCreater : Singleton<ThreadKeyCreater>
{
public:
    ThreadKeyCreater();
    ~ThreadKeyCreater();
public:
    typedef void (*FNDestroyKeyValue)(void* p);
    //single thread
    int  Init(int iKeyPoolSize);
    //single thread
    void Destroy();
    pthread_key_t * CreateKey(FNDestroyKeyValue fndestroy);
private:
    Mutex   mutex;
    int iMaxKeyNum;
    int iNextIdx;
    pthread_key_t * keyPool;
};


//init routine
    //pthread_create key
//


//must be in global life circle
class ThreadLocalStorage
{
public:
    int      Init(pthread_key_t k);
    void*    GetLocalValue();
    int      SetLocalValue(const void* val);
private:
    pthread_key_t    key;
};

