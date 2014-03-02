/*
 * =====================================================================================
 *
 *       Filename:  Socket.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2014年05月22日 05时03分51秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *   Last Changed: 05/22/2014 06时27分20秒
 *
 * =====================================================================================
 */
#include "Socket.h"
#include "base/Log.h"
#include "base/CommonMacro.h"

SockAddress::SockAddress()
{
    Construct(0,NULL);

}
SockAddress::SockAddress(const sockaddr_in & addr_in)
{
    sock_addr = addr_in;
    bIsEmpty=false;
}
SockAddress::SockAddress(const sockaddr & addr)
{
    memcpy(&sock_addr,&addr,sizeof(sock_addr));
    bIsEmpty=false;
}   
SockAddress::SockAddress(uint16_t wPort,char* pszIpOrDomainName)
{
    Construct(wPort,pszIpOrDomainName);
}


void  SockAddress::Construct(uint16_t wPort,char* pszIpOrDomainName)
{
	bzero((char*)&sock_addr,sizeof(sock_addr));
	bIsEmpty = true;
	sock_addr.sin_family = AF_INET;
	sock_addr.sin_port = htons(wPort);
	if(NULL == pszIpOrDomainName)
	{
		sock_addr.sin_addr.s_addr = INADDR_ANY;
	}
	else
	{
		sock_addr.sin_addr.s_addr = inet_addr(pszIpOrDomainName);			
		if(sock_addr.sin_addr.s_addr == (uint32_t)-1)
		{
			struct hostent *host = gethostbyname(pszIpOrDomainName);			
			if (host != NULL && host->h_length > 0) 
			{
				LOG_INFO("get ip by host name = %s",pszIpOrDomainName);
				sock_addr.sin_addr = *(in_addr*)host->h_addr_list[0];
			}
             else
             {
                LOG_ERROR("unknown sock address = %s ",pszIpOrDomainName);
                return;
             }
		}
	}
	bIsEmpty = false;
}
const char* SockAddress::ToString()
{
    in_addr addr;
    addr.s_addr = GetIP();
	snprintf(szSockAddrBuffer,
					sizeof(szSockAddrBuffer),"%s:%d",
					inet_ntoa(addr),
					GetPort());	
	return szSockAddrBuffer;
}

uint16_t    SockAddress::GetPort() const
{
	return sock_addr.sin_port;
}
uint32_t    SockAddress::GetIP()   const
{
	return sock_addr.sin_addr.s_addr;
}

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

//--------------------------------------------









