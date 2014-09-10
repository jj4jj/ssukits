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
 *   Last Changed: 03/11/2014 21时10分57秒
 *
 * =====================================================================================
 */


#pragma once
#include "base/stdinc.h"
#include "base/Buffer.h"

class SockAddress
{
public:
	SockAddress();
	SockAddress(const sockaddr_in & addr_in);
	SockAddress(const sockaddr & addr);	
	SockAddress(uint16_t wPort,const char* pszIpOrDomainName = NULL);
    SockAddress(const char* pszUnixDomainPath);
	void  ConstructIN4(uint16_t wPort,const char* pszIpOrDomainName);
	const char* ToString() ;
	uint16_t    GetPort() const;
	uint32_t    GetIP()   const;
    uint32_t    GetSockAddrLen() const;
    //if return AF_UNSPEC is invalid address
    //other return AF_INET/INET6/UNIX
    uint16_t    GetAddressType() const;    
public:
    union sock_addr_union
    {
        struct sockaddr_in  addr_in4;//ipv4
        struct sockaddr_in6 addr_in6;//ipv6
        struct sockaddr_un  addr_un;//unix domain
    };
    sock_addr_union addr;
private:	
	char szSockAddrBuffer[64];
};

class Socket
{
public:
	Socket(int fd = -1);
	virtual ~Socket();
	// return 0 is ok. otherwise is fail.
	virtual int	    Init();
    virtual void    Close();    
public:
    // default implementation
	// return < 0 is error. 0 is ok.
	// 1 is close by peer
	virtual int		Send(const Buffer & sendBuffer, int iFlags = 0);			
	virtual int		Recv(Buffer& recvBuff , int iFlags = 0);	
public:
	void	SetFD(int iFd_){iFd = iFd_;}
	int	    GetFD() const {return iFd;}
	int	    SetNonBlock(bool bSet);	
    int     ReuseAddr(bool bSet);
    int     SetOption(int name,void* pValBuff,int iValBuff); 
	int	    SetFlag(int iFlag,bool bSetOpen);
    int     Bind(const SockAddress & local);
    //return 0 , connected ok
    //return 1 , connecting
    //< 0:  error
	int	    ConnectTo(const SockAddress & remote);
    //must call after connect to ok.
	SockAddress GetPeerAddress();
    //must call after bind
	const 	SockAddress  GetLocalAddress();
    //get socket error state
    int     GetErrorState();
private:
	SockAddress	localAddress;
	int		iFd;//-1
};




