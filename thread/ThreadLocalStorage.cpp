#include "base/Log.h"
#include "base/CommonMacro.h"

#include "ThreadLocalStorage.h"

#if 1
ThreadKeyCreater::ThreadKeyCreater()
{
    iMaxKeyNum = 0;
    iNextIdx = 0;   
    keyPool = NULL;
    mutex.Init(NULL);
}
ThreadKeyCreater::~ThreadKeyCreater()
{
    Destroy();
}
//single thread
int ThreadKeyCreater::Init(int iKeyPoolSize)
{
    iMaxKeyNum = iKeyPoolSize;
    keyPool = (pthread_key_t*)malloc(sizeof(pthread_key_t)*iMaxKeyNum);
    iNextIdx = 0;
    mutex.Init(NULL);
}
//single thread
void ThreadKeyCreater::Destroy()
{
    iMaxKeyNum = 0;
    iNextIdx = 0;                
    mutex.Destroy();
    SAFE_FREE(keyPool);
}
pthread_key_t * ThreadKeyCreater::CreateKey(FNDestroyKeyValue fndestroy)
{
    //
    mutex.Lock();
    if(iNextIdx >= iMaxKeyNum)
    {
        mutex.Unlock();
        return NULL;
    }
    pthread_key_create(&keyPool[iNextIdx],fndestroy);        
    ++iNextIdx;        
    mutex.Unlock();
    return &keyPool[iNextIdx];
}    




#endif



#if 1
int ThreadLocalStorage::Init(pthread_key_t k)
{
    key = k;
    return 0;
}    
void*    ThreadLocalStorage::GetLocalValue()
{
    return pthread_getspecific(key);
}
int      ThreadLocalStorage::SetLocalValue(const void* val)
{
    return pthread_setspecific(key,val);
}
#endif
