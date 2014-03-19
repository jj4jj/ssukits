
#include "ThreadSync.h"



Mutex::Mutex()
{
    mutex = PTHREAD_MUTEX_INITIALIZER;
}
void Mutex::SetMutex(mutex_t _mu)
{
    mutex = _mu;
}
int Mutex::Init(const pthread_mutexattr_t *restrict attr)
{
    return  pthread_mutex_init(&mutex,
            attr);
}
int Mutex::Destroy()
{
    return pthread_mutex_destroy(&mutex);
}    
int Mutex::Lock()
{
    return pthread_mutex_lock(&mutex);
}
int Mutex::Unlock()
{
    return pthread_mutex_unlock(&mutex);
}
int Mutex::Trylock()
{
    return  pthread_mutex_trylock(&mutex);
}


////////////////////////////////////////////////////////////////////////////////////

Condition::Condition()
{
     cond = PTHREAD_COND_INITIALIZER;
}
void Condition::SetCond(pthread_cond_t _cond)
{
    cond = _cond;
}
int Condition::Init(const pthread_condattr_t *restrict attr)
{
    return pthread_cond_init(&cond,attr);
}
int Condition::TimeWaitMutex(Mutex& mutex,const struct timespec *restrict abstime)
{
    return pthread_cond_timedwait(&cond,
                &(mutex.mutex),abstime);
}
int Condition::WaitMutex(Mutex& mutex)
{
    return pthread_cond_wait(&cond,
               &(mutex.mutex));
}
int Condition::NotifyAll()
{
    return pthread_cond_broadcast(&cond);
}
int Condition::Notify()
{
    return pthread_cond_signal(&cond);
}
int Condition::Destroy()
{
    return pthread_cond_destroy(&cond);
}

////////////////////////////////////////////////////////////////////////////////////

void    Semaphore::SetSemphore(sem_t _sem)
{
    sem = _sem;
}
int Semaphore::Init(int pshared, unsigned int value)
{
    return sem_init(&sem,pshared, value);
}
int Semaphore::Wait()
{
    return sem_wait(&sem);
}
int Semaphore::TryWait()
{
    return sem_trywait(&sem);
}
int Semaphore::Post()
{
    return sem_post(&sem);
}
int Semaphore::Destroy()
{
    return sem_destroy(&sem);
}

////////////////////////////////////////////////////////////////////////////////////


