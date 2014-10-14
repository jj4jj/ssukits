/*
 * =====================================================================================
 *
 *       Filename:  Singleton.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2014年05月22日 07时49分47秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *   Last Changed: 05/26/2014 07时49分09秒
 *
 * =====================================================================================
 */

#pragma once
#include "NonCopyable.hpp"
#include <pthread.h>
#include <memory>
//single thread edition 
//multi thread edition
//

////////////////////////////////////////////////////
using std::auto_ptr;

template <class T>
class Singleton
{
public :
    static inline T* instance();
    static inline T& Instance()
    {
        return *instance();
    }
protected :
    Singleton(void){}
    ~Singleton(void){}
    Singleton(const Singleton&){}
    Singleton & operator = (const Singleton &){} 
    static auto_ptr<T> _instance;
};
 
template <class T>
auto_ptr<T> Singleton<T>::_instance;
 
template <class T>
inline T* Singleton<T>::instance()
{
   if( 0 == _instance.get())
   {
        static pthread_mutex_t  mutex = PTHREAD_MUTEX_INITIALIZER;
        pthread_mutex_lock(&mutex);
        if(0 == _instance.get())
        {
            _instance.reset ( new T);
        }
        pthread_mutex_unlock(&mutex);
   }  
   return _instance.get();
}


/////////////////////////////////////////////////////////////////////////////
#define DeclareDefaultPrivateConstructor(clsName)  \
private:\
    clsName(){}\
    ~clsName(){}
#define DeclareSingltonSupport(clsName)  \
    friend class auto_ptr<clsName>;\
    friend class Singleton<clsName>;



