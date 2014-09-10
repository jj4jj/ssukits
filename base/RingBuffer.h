#pragma once
#include "stdinc.h"
#include "Buffer.h"
/////////////////////////////////////////////////////

class RingBuffer
{
public:
    //return 0 is ok
    int     Init(int cap);
    void    Destroy();
    //return 0 is ok
    int     Read(Buffer & buffer);
    //return 0 is ok
    int     Write(const Buffer& buffer);    
    int     GetAvailBytes();
public:
    RingBuffer();
private:
    char* pBuffer;
    int   iHead;
    int   iTail;
    int   iCap;
};









