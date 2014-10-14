#include "Buffer.h"
#include "Log.h"

#include "CommonMacro.h"
//--------------------------------------------
Buffer::Buffer()
{
	memset(this,0,sizeof(*this));
}
Buffer::Buffer(const char* pszString)
{
	pBuffer = (uint8_t *)pszString;
	iCap = 0;
	iUsed = strlen(pszString) + 1;
}
Buffer::Buffer(char* pszBuffer,int iCap_)
{
    if(iCap_ <= 0)
    {
        return ;
    }
    pBuffer = (uint8_t *)pszBuffer;
    iUsed = iCap = iCap_;
}
Buffer::Buffer(const Buffer & bf)
{
    if(this != &bf)
    {
        *this = bf;
    }
}
int Buffer::Create(int iCapSet)
{
    if(iCapSet <= 0)
    {
        return -1;
    }
    pBuffer = (uint8_t*)malloc(iCapSet);
    if(NULL == pBuffer)
    {        
        LOG_ERROR("malloc error for size = %d !",iCapSet);
        return -1;
    }
    iCap = iCapSet;
    iUsed = 0;
    return 0;
}
void Buffer::Destroy()
{
    SAFE_FREE(pBuffer);
    iCap = 0;
    iUsed = 0;
}

