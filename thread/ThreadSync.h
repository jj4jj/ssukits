#pragma once

#include "base/stdinc.h"


/////////////////////////////////////////////////////////////////


class Mutex
{
public:
    Mutex();
    void SetMutex(pthread_mutex_t _mu);
    pthread_mutex_t & GetMutex();
    int Init(const pthread_mutexattr_t *restrict attr);
    int Destroy();
    int Lock();
    int Unlock();
    int Trylock();
private:
    pthread_mutex_t mutex;
};

/////////////////////////////////////////////////////////////////

class Condition
{
public:
    Condition();
    void SetCond(pthread_cond_t _cond);
    int Init(const pthread_condattr_t *restrict attr);
    int TimeWaitMutex(Mutex& mutex,const struct timespec *restrict abstime);
    int WaitMutex(Mutex& mutex);
    int NotifyAll();
    int Notify();
    int Destroy();
private:
    pthread_cond_t  cond;
};


/////////////////////////////////////////////////////////////////

class Semaphore
{
public:
    void    SetSemphore(sem_t _sem);
    int Init(int pshared, unsigned int value);
    int Wait();
    int TryWait();
    int Post();
    int Destroy();
private:
    sem_t sem;
};


///////////////////////////////////
class RWLock
{
public:
    int Init(pthread_rwlockattr_t * attr);
    int Destroy();
    int GetReadLock();
    int GetWriteLock();
    int TryGetReadLock();
    int TryGetWriteLock();
    int ReleaseLock();
    
private:
    pthread_rwlock_t   rwlock;
};




