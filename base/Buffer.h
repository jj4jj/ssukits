#pragma once
#include "stdinc.h"

struct Buffer
{
	uint8_t * pBuffer;//buffer pointer
	int32_t	  iCap;//capcity , if cont pBuffer , cap is 0.
	int32_t	  iUsed;//the valid data length (by bytes)
public:
	Buffer();
    Buffer(const Buffer & bf);
	Buffer(const char* pszString);
    Buffer(char* pszBuffer,int iCap_);
    //max pair appearence
    int  Create(int iCapSet);
    void Destroy();
};





