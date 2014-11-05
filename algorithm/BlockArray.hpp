#pragma once

#include "base/stdinc.h"

template<class T>
class BlockArray
{
public:    
    typedef T   value_type;
    typedef T * pointer; 
    typedef T & reference; 
    typedef const T & const_reference;
    typedef const T * const_pointer;
    typedef pointer iterator;
private:
    size_t     &    zNum;
    size_t          zMax;
    value_type *    data;
public:
    BlockArray(T * pData,size_t & zNum_,size_t zMax_):zNum(zNum_),zMax(zMax_),data(pData){}
public:
    inline iterator  begin()
    {
        return  data;
    }
    inline iterator  end()
    {
        return (data + zNum );
    }
    void  erase(const iterator it)
    {
        if(it >= data + zNum || it < data )
        {
            return;
        }
        memcpy(it,it+1,data+zNum - it - 1);
        zNum--;
    }
    void  erase(const iterator  begp,const iterator  endp)
    {
        if(begp < begin() || endp > end() )
        {
            return;
        }        
        memcpy(begp,endp,end() - endp);
        zNum -= (endp - begp);         
    }   
    iterator  insert(const iterator  pos,const_reference val)
    {
        if(zNum >= zMax || pos > data + zNum || pos < data )
        {
            return end();
        }        
        //
        for(iterator xend = end();xend > pos; --xend)
        {
            *xend = *(xend-1);
        }
        //memmove(pos+1,pos,data + zNum - pos );
        *pos = val;
        ++zNum;
        return pos;
    }
    reference operator [](size_t idx)
    {        
        return data[idx];
    }
    template <class Itr>
    void assign(Itr first,Itr last)
    {
        zNum = 0;
        while(first != last && zNum < zMax)
        {
            data[zNum++] = *first;
            ++first;
        }
    }
};


