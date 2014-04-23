#pragma once
#include "Buffer.h"





template<class Type>
struct Hash
{
    size_t  operator(const Type v);
};


//todo
////////////////////////////////////////////////
template<>
struct Hash<string>
{
    size_t operator(const string & s)
    {
        ///
        return 0;
    }
};

////////////////////////////////////////////////
template<>
struct Hash<const char*>
{
    size_t operator(const char * s)
    {
        ///
        return 0;
    }
};

////////////////////////////////////////////////
template<>
struct Hash<Buffer>
{
    size_t operator(const Buffer & buffer)
    {
        ///
        return 0;
    }
};







