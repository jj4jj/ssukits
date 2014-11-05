#pragma once
#include "BlockArray.hpp"

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



