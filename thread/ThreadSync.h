#include "base/stdinc.h"


class Mutex
{
public:
    Mutex()
    {
        mutex = PTHREAD_MUTEX_INITIALIZER;
    }
    void SetMutex(mutex_t _mu)
    {
        mutex = _mu;
    }
    int Init(const pthread_mutexattr_t *restrict attr)
    {
        return  pthread_mutex_init(&mutex,
                attr);
    }
    int Destroy()
    {
        return pthread_mutex_destroy(&mutex);
    }    
    int Lock()
    {
        return pthread_mutex_lock(&mutex);
    }
    int Unlock()
    {
        return pthread_mutex_unlock(&mutex);
    }
    int Trylock()
    {
        return  pthread_mutex_trylock(&mutex);
    }
private:
    pthread_mutex_t mutex;
};

class Condition
{
public:
    Condition()
    {
         cond = PTHREAD_COND_INITIALIZER;
    }
    void SetCond(pthread_cond_t _cond)
    {
        cond = _cond;
    }
    int Init(const pthread_condattr_t *restrict attr)
    {
        return pthread_cond_init(&cond,attr);
    }
    int TimeWaitMutex(Mutex& mutex,const struct timespec *restrict abstime)
    {
        return pthread_cond_timedwait(&cond,
                    &(mutex.mutex),abstime);
    }
    int WaitMutex(Mutex& mutex)
    {
        return pthread_cond_wait(&cond,
                   &(mutex.mutex));
    }
    int NotifyAll()
    {
        return pthread_cond_broadcast(&cond);
    }
    int Notify()
    {
        return pthread_cond_signal(&cond);
    }
    int Destroy()
    {
        return pthread_cond_destroy(&cond);
    }
private:
    pthread_cond_t  cond;
};

class Semaphore
{
public:
    void    SetSemphore(sem_t _sem)
    {
        sem = _sem;
    }
    int Init(int pshared, unsigned int value)
    {
        return sem_init(&sem,pshared, value);
    }
    int Wait()
    {
        return sem_wait(&sem);
    }
    int TryWait()
    {
        return sem_trywait(&sem);
    }
    int Post()
    {
        return sem_post(&sem);
    }
    int Destroy()
    {
        return sem_destroy(&sem);
    }
private:
    sem_t sem;
};






