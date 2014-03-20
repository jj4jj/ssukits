#include "ThreadLocalStorage.h"
#include "base/Log.h"


ThreadLoacalStorage::ThreadLocalStorage():create_key_once(PTHREAD_ONCE_INIT)
{

}
int ThreadLoacalStorage::Init(FNDestroyValue _fnDestroyValue)
{
    fnDestroyValue = _fnDestroyValue;
    pthread_once(&create_key_once,CreateKey);
}    
void*    ThreadLoacalStorage::GetLocalValue()
{
    return pthread_getspecific(key);
}
int      ThreadLoacalStorage::SetLocalValue(const void* val)
{
    return pthread_setspecific(key,val);
}
static int ThreadLoacalStorage::ThreadLoacalStorage::CreateKey()
{
    int ret = pthread_key_create(&key,fnDestroyValue);
    if( ret < 0)
    {
        LOG_ERROR("pthread_key_create error no = %d",errno);
    }
    return ret;
}

