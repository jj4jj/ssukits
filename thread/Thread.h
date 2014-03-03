#pragma once
#include "base/NonCopyable.hpp"


//a simple thread wrapper
//Thread init , start with a function .
//

struct ThreadFunction
{        
};

int 
typedef void* (*ThreadCallBack)(void*);
class Thread : NonCopyable
{
public:
     //create a thread , yield it.
     int    Create(pthread_attr_t * restrict attr,ThreadCallBack tcb,void* pTcbArg)
     {
        pThreadCallBackArg = pTcbArg;
        pfnThreadCallBack = tcb;
        int ret = pthread_create(&tid,
        	        attr,
        	        pfnThreadCallBack,pTcbArg);
        if(ret == 0)
        {
            LOG_DEBUG("thread id = %d is created ok !",tid);
        }
        else
        {
            LOG_ERROR("thread id created fail ret = %d !",ret);
        }
        return ret;
     }
     pthread_t  GetThreadID();
     int    Start();
     int    Join();
     int    Resume();
     int    Murder();
     int    Suicide();
     int    Detach();
public:
    pthread_t   GetCurrentThreadID()
    {
        return pthread_self();
    }
private:
    pthread_t       tid;
    ThreadCallBack  pfnThreadCallBack;
    void*           pThreadCallBackArg;
};






