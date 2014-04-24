#pragma once

class RandomGenerator : Singleton<RandomGeneator>
{    
public:
    //segment is [min,max]
    template<class T>
    T      Random(T min,T max)
    {
        
        if(min > max)
        {
            SWAP(min,max);
        }
        else if(max == min)
        {
            return min;
        }
        return (T)rand()%(max - min + 1) + min; 
    }        
protected:
    RandomGenerator(uint32_t dwSeed = 0)
    {
        if(0 == dwSeed)
        {
            dwSeed = time(NULL);
        }
        SetSeed(dwSeed)        
    }
    int SetSeed(uint32_t dwSeed)
    {
        srand(dwSeed);
    }
};












