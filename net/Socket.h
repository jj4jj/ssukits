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


#pragma once
#include "base/stdinclude.h"


class SockAddress
{
public:
	SockAddress():bIsEmpty(true){}
	SockAddress(const sockaddr_in & addr_in){sock_addr = addr_in;bIsEmpty=false;}
	SockAddress(const sockaddr & addr)sock_addr{memset(&sock_addr,&addr,sizeof(sock_addr));bIsEmpty=false;}	
	SockAddress(uint16_t wPort,char* pszIpOrDomainName){Construct(wPort,pszIpOrDomainName);}
	void Construct(uint16_t wPort,char* pszIpOrDomainName);
	const char* ToString() const;
	uint16_t    GetPort() const;
	uint32_t    GetIP()   const;
public:
	sockaddr_in	sock_addr;
	bool		bIsEmpty;
private:	
	char szSockAddrBuffer[32];
};
struct Buffer
{
	uint8_t * pBuffer;//buffer pointer
	int32_t	  iCap;//capcity , if cont pBuffer , cap is 0.
	int32_t	  iUsed;//the valid data length (by bytes)
	Buffer();
	Buffer(const char* pszString);
    Buffer(char* pszBuffer,int iCap_);
};

class Socket
{
public:
	Socket():iFd(-1){}
	virtual Socket(){}
	
	// return 0 is ok. otherwise fail.
	int		Init(){iFd = -1;return 0;}	
	// return 0 is ok. otherwise is fail.
	virtual int	Send(const Buffer & buffer) = 0;			
	virtual int	Recv(Buffer& recvBuff) = 0;

	// return 0 is ok. otherwise fail.
	int	SetLocalAddress(const SockAddress & ip){localAddress = ip;}
	int	SetPeerAddress(const SockAddress & ip){peerAddress = ip;}     
	const 	SockAddress &	GetLocalAddress(){return localAddress;}
	const 	SockAddress &	GetPeerAddress(){return peerAddress;}
	void	SetFD(int iFd_){iFd = iFd_;};
	int	GetFD() const {return iFd;}
private:
	SockAddress	localAddress;
	SockAddress	peerAddress;
	int		iFd;//-1
};
