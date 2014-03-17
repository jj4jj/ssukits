#pragma once
#include "base/NonCopyable.hpp"


//a simple thread wrapper
//Thread init , start with a function .
//
typedef void* (*ThreadCallBack)(void*);
class Thread : NonCopyable
{
public:
     Thread()
     {
        thread_id = 0;
     }
     void    SetID(pthread_t _tid)
     {
        thread_id = _tid;
     }
     //create a thread , yield it.
     int    Create(pthread_attr_t * restrict attr,ThreadCallBack tcb,void* pTcbArg)
     {
        int ret = pthread_create(&thread_id,
        	        attr,
        	        tcb,pTcbArg);
        if(ret == 0)
        {
            LOG_DEBUG("thread id = %d is created ok !",thread_id);
        }
        else
        {
            LOG_ERROR("thread id created fail ret = %d !",ret);
        }
        return ret;
     }
     pthread_t  GetThreadID()
     {
        return thread_id;
     }
     // current thread join this thread
     int    Join(void **retval = NULL)
     {
        return pthread_join(thread_id,retval);
     }
     int    Stop()
     {
         return pthread_cancel(thread_id);
     }
     int    Detach()
     {
         return  pthread_detach(thread_id);
     }
public:
    static pthread_t   GetCurrentThreadID()
    {
        return pthread_self();
    }
    static void     ExitCurrentThread(void* retval)
    {
         return pthread_exit(retval);
    }
private:
    pthread_t       thread_id;
};


