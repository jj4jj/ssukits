#pragma once

#include "base/stdinc.h"

template<class U , class Compare = std::less<U> >
class BinarySearchArray
{
protected:
    typedef    typename std::vector<U>       ListType;
    typedef    typename ListType::iterator   ListTypeItr;
    
public:    

    template<class Itr>
    //assign a container to list
    void    Assign(Itr first,Itr last)
    {
        list.assign(first,last);
        Compare comp;
        std::sort(list.begin(),list.end(),comp);
    }
    //construct a empty list
    void    Reserve(int iReserveSize = 16)
    {
        list.reserve(iReserveSize);
    }

    //first not less than u element
    //if there is no this elements , return end();
    ListTypeItr LowwerBound(const U & u)
    {
    //   return lowwer_bound(list.begin(),list.end(),u);
        Compare comp;
        ListTypeItr it = list.begin(),
                    first = list.begin();
        typename std::iterator_traits<ListTypeItr>::difference_type count, step;
        
        count = std::distance(first,list.end());
        while (count > 0)
		{
            it = first;
            step = count>>1;
            std::advance(it, step);
            if (comp(*it,u)) 
            {
                first = ++it;
                count -= step + 1;
            } 
            else
            {
                count = step;
            }
        }
        return first;        
    }
    //first greater than u elements
    //if there is no this elements , return end();
    ListTypeItr UpperBound(const U & u)
    {
    //   return upper_bound(list.begin(),list.end(),u);
        Compare comp;
        ListTypeItr it = list.begin(),
                    first = list.begin();
        typename std::iterator_traits<ListTypeItr>::difference_type count, step;
        
        count = std::distance(first,list.end());     
        while (count > 0) {
            it = first;
            step = count>>1;
            std::advance(it, step);
            if(!comp(*it,u)) 
            {
                first = ++it;
                count -= step + 1;
            } 
            else
            {
                count = step;
            }
        }
        return first; 
    }
    ListTypeItr End()
    {
        return list.end();
    }
    
    //o(log(N))
    //return > 0 is repeat
    //return 0 is ok
    //return < 0 is ilegal
    int     Insert(const U& u , bool bInsertWhenEual = false )
    {

        //lowwer_bound it
        ListTypeItr it = UpperBound(u);
        if(it == End())
        {
            Append(u);
            return 0;
        }
        if(it != list.begin())
        {
            //insert u into the it pos        
            if(!bInsertWhenEual && Equal(*(it-1),u))
            {
                //repeat
                return -1;
            }        
        }
        //insert it
        list.insert(it,u);
    }    
    bool    Equal(const U& u1,const U& u2)
    {
        return !Compare()(u1,u2) && !Compare()(u2,u1);
    }
    void     Append(const U& u)
    {
        if(!list.empty())
        {
            assert(!Compare()(u,list.back()));
        }
        list.push_back(u);
    }
    //o(N)
    int     Remove1st(const U& u)
    {
        ListTypeItr it = LowwerBound(u);
        if(it != End())
        {
            if(Equal(*it,u))
            {
                //
                list.erase(it);
                return 0;
            }
        }
        //not found
        return -1;
    }
    int     RemoveAll(const U& u)
    {
        ListTypeItr it1 = LowwerBound(u);
        ListTypeItr it2 = UpperBound(u);
        if(it1 != it2)
        {
            list.erase(it1,it2);
            return 0;
        }
        return -1;
    }
    //o(logN)
    U*      Find1st(const U & u)
    {
        ListTypeItr it = LowwerBound(u);
        if(it != End())
        {
            if(Equal(*it,u))
            {
                return &(list.at(it-list.begin()));
            }
        }
        return NULL;
    }
    void     FindAll(const U & u,std::vector<U> & rlst)
    {
        rlst.clear();
        ListTypeItr it = LowwerBound(u);
        while(it != End() &&
               Equal(*it,u))
        {
            rlst.push_back(*it);
        }
    }
private:
    std::vector<U>  list;
};



