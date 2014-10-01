
#include "base/Log.h"
#include "Thread.h"

#if 1
Thread::Thread()
{
    thread_id = 0;
}
void    Thread::SetID(pthread_t _tid)
{
    thread_id = _tid;
}
#endif

#if 1
//create a thread , yield it.
int    Thread::Create(pthread_attr_t * restrict attr,ThreadCallBack tcb,void* pTcbArg)
{
    int ret = pthread_create(&thread_id,
	        attr,
	        tcb,pTcbArg);
    if(ret == 0)
    {
        LOG_DEBUG("thread id = %u is created ok !",thread_id);
    }
    else
    {
        LOG_ERROR("thread id created fail ret = %d !",ret);
    }
    return ret;
}
pthread_t  Thread::GetThreadID()
{
    return thread_id;
}
// current thread join this thread
int    Thread::Join(void **retval )
{
    return pthread_join(thread_id,retval);
}
int    Thread::Stop()
{
    return pthread_cancel(thread_id);
}
int    Thread::Detach()
{
    return  pthread_detach(thread_id);
}

pthread_t   Thread::GetCurrentThreadID()
{
    return pthread_self();
}
void     Thread::ExitCurrentThread(void* retval)
{
     return pthread_exit(retval);
}
#endif
















