
#pragma once

#include "base/stdinc.h"

template<class U,class Compare = std::less<U> >
class Heap
{
public:
    template< class ForwardIt >
    int     Push(ForwardIt first,ForwardIt last)
    {
        ForwardIt it = first;
        while(it != last)
        {
            Push(*it);
            ++it;
        }
        return 0;
    }
    int     Push(const U & u)
    {
        heap.push_back(u);
        std::push_heap(heap.begin(),heap.end(),Compare());
        return 0;
    }
    int     Pop(U& u)
    {
        if(heap.empty())
        {
            return -1;
        }
        std::pop_heap(heap.begin(),heap.end(),Compare());
        u = heap.back();
        heap.pop_back();
        return 0;
    }
    int    Count()
    {
        return heap.size();
    }
    template< class Compare >
    void     Sort(vector<U> & sortedHeap)
    {
        sortedHeap = heap;        
        sort_heap(sortedHeap.begin(),sortedHeap.end(),Compare());        
    }
private:       
    std::vector<U>  heap;
};


