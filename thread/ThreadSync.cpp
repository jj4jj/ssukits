
#include "ThreadSync.h"



Mutex::Mutex()
{
    pthread_mutex_t tmp = PTHREAD_MUTEX_INITIALIZER;
    mutex = tmp;
}
void Mutex::SetMutex(pthread_mutex_t _mu)
{
    mutex = _mu;
}
pthread_mutex_t & Mutex::GetMutex()
{
    return mutex;
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
    pthread_cond_t tmp = PTHREAD_COND_INITIALIZER;
    cond = tmp;   
}
void Condition::SetCond(pthread_cond_t _cond)
{
    cond = _cond;
}
int Condition::Init(const pthread_condattr_t *restrict attr)
{
    return pthread_cond_init(&cond,attr);
}
int Condition::TimeWaitMutex(Mutex & mutex,const struct timespec *restrict abstime)
{
    return pthread_cond_timedwait(&cond,
                &(mutex.GetMutex()),abstime);
}
int Condition::WaitMutex(Mutex& mutex)
{
    return pthread_cond_wait(&cond,
               &(mutex.GetMutex()));
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


int RWLock::Init(pthread_rwlockattr_t * attr)
{
    return pthread_rwlock_init(&rwlock,attr);
}
int RWLock::Destroy()
{
    return pthread_rwlock_destroy(&rwlock);        
}
int RWLock::GetReadLock()
{
    return pthread_rwlock_rdlock(&rwlock);
}
int RWLock::GetWriteLock()
{
    return pthread_rwlock_wrlock(&rwlock);
}
int RWLock::TryGetReadLock()
{
    return pthread_rwlock_tryrdlock(&rwlock);
}
int RWLock::TryGetWriteLock()
{
    return pthread_rwlock_trywrlock(&rwlock);
}
int RWLock::ReleaseLock()
{
    return pthread_rwlock_unlock(&rwlock);
}
