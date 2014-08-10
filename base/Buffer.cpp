#include "Buffer.h"


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
    pBuffer = (uint8_t*)malloc(iCapSet);
    if(NULL == pBuffer)
    {
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



/////////////////////////////////////////////////////


//return 0 is ok
int     RingBuffer::Init(int cap)
{
    if(cap <= 0)
    {
        return -1;
    }
    Destroy();        
    pBuffer = (char*)malloc(cap);
    iHead = iTail = 0;
    iCap = cap;
    return 0;
}
void    RingBuffer::Destroy()
{
    SAFE_FREE(pBuffer);
    iHead = iTail = iCap = 0;
}
//return 0 is ok
int     RingBuffer::Read(Buffer & buffer)
{
    if(iHead == iTail)
    {
        //empty
        return -1;
    }
    int iRead = MIN(buffer.iCap,
                (iTail-iHead+iCap)%iCap);
    memcpy(buffer.pBuffer,
           pBuffer,
           iRead);
    buffer.iUsed = iRead;        
    iHead = (iHead+iRead)%iCap;
    return 0;
}
//return 0 is ok
int     RingBuffer::Write(const Buffer& buffer)
{
    if(buffer.iUsed > GetAvailBytes())
    {
        //not enough space to filling
        return -1;
    }
    int iWrite = MIN(buffer.iUsed,iCap-iTail);
    memcpy(pBuffer+iTail,buffer.pBuffer,iWrite);
	int iBfUsed = buffer.iUsed;
    iBfUsed -= iWrite;
    memcpy(pBuffer,buffer.pBuffer+iWrite,iBfUsed);        
    iTail = (iTail + iWrite + iBfUsed)%iCap;
    return 0;
}
int     RingBuffer::GetAvailBytes()
{
    return  (iHead - iTail + iCap )%iCap;
}
RingBuffer::RingBuffer()
{
    iHead = iTail = iCap = 0;            
    pBuffer = NULL;
}



