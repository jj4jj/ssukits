#pragma once

#include "base/stdinc.h"


/////////////////////////////////////////////////////////////////


class Mutex
{
public:
    Mutex()
    void SetMutex(mutex_t _mu);
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






