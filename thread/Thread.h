#pragma once
#include "base/NonCopyable.hpp"
#include "base/stdinc.h"

//a simple thread wrapper
//Thread init , start with a function .
//

#define restrict        

typedef void* (*ThreadCallBack)(void*);
class Thread
{
public:
     Thread();     
     void    SetID(pthread_t _tid);
     //create a thread , yield it.
     int    Create(pthread_attr_t * restrict attr,ThreadCallBack tcb,void* pTcbArg);
     pthread_t  GetThreadID();
     // current thread join this thread
     int    Join(void **retval = NULL);
     int    Stop();
     int    Detach();
public:
    static pthread_t   GetCurrentThreadID();
    static void        ExitCurrentThread(void* retval);
    static void        usleep(int   us){usleep(us);}
private:
    pthread_t       thread_id;
};







