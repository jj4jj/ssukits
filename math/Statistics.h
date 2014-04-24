#pragma once

class Statistics
{
public:
    template<class Itr,class T>
    T   Average(Itr first,Itr last)
    {
        return Sum(first,last)/distance(first,last);
    }
    template<class Itr,class T>
    T   Sum(Itr first,Itr last)
    {
        T sum = 0;
        for_each(first,last,&sum,&sum,std::plus<T>());
        return sum;
    }
     
    //////////////////////////////////////////////



};






