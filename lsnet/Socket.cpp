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




SockAddress::SockAddress(char* pszIp,uint16_t wPort)
{
	bzero((char*)&mysock,sizeof(mysock));
	sock_addr.sa_family=AF_INET;
	sock_addr.sin_port=htons(wPort);//1234�Ƕ˿ں�
	sock_addr.sin_addr.s_addr=inet_addr(pszIp);
}
const char* SockAddress::ToString() const
{
	snprintf(szSockAddrBuffer,
					sizeof(szSockAddrBuffer),
					"%s:%d",GetIP(),GetPort());	
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
