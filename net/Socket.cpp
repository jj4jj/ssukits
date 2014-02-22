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

void  SocketAddress::Construct(uint16_t wPort,char* pszIpOrDomainName)
{
	bzero((char*)&mysock,sizeof(mysock));
	bIsEmpty = true;
	sock_addr.sa_family=AF_INET;
	sock_addr.sin_port = htons(wPort);//1234�Ƕ˿ں�
	if(NULL == pszIp)
	{
		sock_addr.sin_addr.s_addr = INADDR_ANY;
	}
	else
	{
		sock_addr.sin_addr.s_addr = inet_addr(pszIp);			
		if(sock_addr.sin_addr.s_add == -1)
		{
			struct hostent *host = gethostbyname(pszIpOrDomainName);			
			if (host != NULL) 
			{
				LOG_INFO("get ip by host name = %s",pszIpOrDomainName);
				sock_addr.sin_addr.s_addr = host->h_addr_list[0];
			}
		}
	}
	bIsEmpty = false;
}
const char* SockAddress::ToString() const
{
	snprintf(szSockAddrBuffer,
					sizeof(szSockAddrBuffer),
					"%s:%d",inet_ntoa(GetIP()),GetPort());	
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