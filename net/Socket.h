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
 *   Last Changed: 05/26/2014 05时52分12秒
 *
 * =====================================================================================
 */


#pragma once
#include "base/stdinc.h"


class SockAddress
{
public:
	SockAddress();
	SockAddress(const sockaddr_in & addr_in);
	SockAddress(const sockaddr & addr);	
	SockAddress(uint16_t wPort,char* pszIpOrDomainName = NULL);
	void Construct(uint16_t wPort,char* pszIpOrDomainName);
	const char* ToString() ;
	uint16_t    GetPort() const;
	uint32_t    GetIP()   const;
public:
	sockaddr_in	sock_addr;
	bool		bIsEmpty;
private:	
	char szSockAddrBuffer[64];
};
struct Buffer
{
	uint8_t * pBuffer;//buffer pointer
	int32_t	  iCap;//capcity , if cont pBuffer , cap is 0.
	int32_t	  iUsed;//the valid data length (by bytes)
	Buffer();
    Buffer(const Buffer & bf);
	Buffer(const char* pszString);
    Buffer(char* pszBuffer,int iCap_);
    //max pair appearence
    int Create(int iCapSet);
    void Destroy();
};

class Socket
{
public:
	// return 0 is ok. otherwise fail.
	int		Init(){iFd = -1;return 0;}	
	// return 0 is ok. otherwise is fail.
	virtual int	Send(const Buffer & buffer) = 0;			
	virtual int	Recv(Buffer& recvBuff) = 0;

	// return 0 is ok. otherwise fail.
	void	    SetLocalAddress(const SockAddress & ip){localAddress = ip;}
	const 	SockAddress &	GetLocalAddress(){return localAddress;}
	void	SetFD(int iFd_){iFd = iFd_;}
	int	GetFD() const {return iFd;}
public:
	Socket():iFd(-1){}
	virtual ~Socket(){}	    
private:
	SockAddress	localAddress;
	int		iFd;//-1
};
