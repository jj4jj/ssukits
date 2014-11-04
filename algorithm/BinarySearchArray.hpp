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
    size_t     *    pzNum;
    size_t          zMax;
    value_type *    data;
public:
    BlockArray(T * pData,size_t * pzNum_,size_t zMax_)
    {
        data = pData;
        zMax = zMax_;
        pzNum = pzNum_;
    }
public:
    inline iterator  begin()
    {
        return  data;
    }
    inline iterator  end()
    {
        return (data + (*pzNum) );
    }
    void  erase(const iterator it)
    {
        if(it >= data + (*pzNum) || it < data )
        {
            return;
        }
        memcpy(it,it+1,data+(*pzNum) - it - 1);
        (*pzNum)--;
    }
    void  erase(const iterator  begp,const iterator  endp)
    {
        if(begp < begin() || endp > end() )
        {
            return;
        }        
        memcpy(begp,endp,end() - endp);
        (*pzNum) -= (endp - begp);         
    }   
    iterator  insert(const iterator  pos,const_reference val)
    {
        if(*pzNum >= zMax || pos > data + (*pzNum) || pos < data )
        {
            return end();
        }        
        //
        for(iterator xend = end();xend > pos; --xend)
        {
            *xend = *(xend-1);
        }
        //memmove(pos+1,pos,data + (*pzNum) - pos );
        *pos = val;
        ++(*pzNum);
        return pos;
    }
    reference operator [](size_t idx)
    {        
        return data[idx];
    }
    template <class Itr>
    void assign(Itr first,Itr last)
    {
        *pzNum = 0;
        while(first != last && *pzNum < zMax)
        {
            data[(*pzNum)++] = *first;
            ++first;
        }
    }
};


template<class U ,class Container = std::vector<U>, class Compare = std::less<U> >
class BinarySearchArray
{
protected:
    typedef      typename Container::iterator   ContainerItr;    
    Container  & list;
    Compare      comp;
public:
    BinarySearchArray(Container & ctn):list(ctn)
    {
        Normalize();
    }
    void Normalize()
    {
        std::sort(list.begin(),list.end(),comp);
    }
    //first not less than u element
    //if there is no this elements , return end();
    inline ContainerItr LowerBound(const U & u)
    {
        return std::lower_bound(list.begin(),list.end(),u,comp);
    }
    //first greater than u elements
    //if there is no this elements , return end();
    inline ContainerItr UpperBound(const U & u)
    {
       return std::upper_bound(list.begin(),list.end(),u,comp);
    }    
    //o(log(N))
    //return > 0 is repeat
    //return 0 is ok
    //return < 0 is ilegal
    inline int     Insert(const U& u , bool bInsertForce = false )
    {
        //lowwer_bound it
        ContainerItr it = LowerBound(u);
        if(it == list.end())
        {
            list.insert(list.end(),u);
            return 0;
        }
        //insert u into the it pos        
        if(!bInsertForce && Equal(*it,u))
        {
            //repeat
            return 1;
        }        
        //insert it
        list.insert(it,u);
        return 0;
    }    
    inline bool    Equal(const U& u1,const U& u2)
    {
        return !comp(u1,u2) && !comp(u2,u1);
    }
    //o(N)
    inline int     Remove1st(const U& u)
    {
        ContainerItr it = LowerBound(u);
        if(it != list.end())
        {
            if(Equal(*it,u))
            {
                list.erase(it);
                return 0;
            }
        }
        //not found
        return -1;
    }
    inline void     RemoveAll(const U& u)
    {
        list.erase(std::remove(list.begin(),list.end(),u),list.end());
    }
    //o(logN)
    inline U*      Find1st(const U & u)
    {
        ContainerItr it = LowerBound(u);
        if(it != list.end())
        {
            if(Equal(*it,u))
            {
                return &(list.at(it-list.begin()));
            }
        }
        return NULL;
    }
    inline void     FindAll(const U & u,std::vector<U> & rlst)
    {
        rlst.clear();
        ContainerItr it = LowerBound(u);
        while(it != list.end() &&
               Equal(*it,u))
        {
            rlst.push_back(*it);
            ++it;
        }
    }
};



