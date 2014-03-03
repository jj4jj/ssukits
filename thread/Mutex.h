#include "base/stdinc.h"


class Mutex
{
public:
    int Init(const pthread_mutexattr_t *restrict attr); 
    int Destroy();
    int Lock();
    int Unlock();
    int Trylock();
private:
    pthread_mutex_t mutex;
};

class Condition
{
public:
    int Init();
    int TimeWaitMutex(Mutex& mutex,const struct timespec *restrict abstime);
    int WaitMutex(Mutex& mutex);
    int BroadCast();
    int Signal();
    int Destroy();
private:
    pthread_cond_t  cond;
};

class Semaphore
{
public:
    int Init(int pshared, unsigned int value);
    int Wait();
    int TryWait();
    int Post();
    int Destroy();
private:
    sem_t sem;
};






