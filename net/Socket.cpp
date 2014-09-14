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
 *   Last Changed: 03/11/2014 21时10分48秒
 *
 * =====================================================================================
 */
#include "Socket.h"
#include "base/Log.h"
#include "base/CommonMacro.h"

SockAddress::SockAddress()
{
	bzero(this,sizeof(*this));    
}
SockAddress::SockAddress(const sockaddr_in & addr_in)
{
    addr.addr_in4  = addr_in;
}
SockAddress::SockAddress(const sockaddr & _addr)
{
    memcpy(&(addr),&_addr,sizeof(_addr));
}   
SockAddress::SockAddress(uint16_t wPort,const char* pszIpOrDomainName)
{
    ConstructIN4(wPort,pszIpOrDomainName);
}

SockAddress::SockAddress(const char* pszUnixDomainPath)
{
    addr.addr_un.sun_family = AF_UNIX;
	strncpy(addr.addr_un.sun_path,pszUnixDomainPath,sizeof(addr.addr_un.sun_path)-1);
}

void  SockAddress::ConstructIN4(uint16_t wPort,const char* pszIpOrDomainName)
{
	bzero((char*)&(addr.addr_in4),sizeof((addr.addr_in4)));
	(addr.addr_in4).sin_port = htons(wPort);
	if(NULL == pszIpOrDomainName)
	{
		(addr.addr_in4).sin_addr.s_addr = INADDR_ANY;
	}
	else
	{
		(addr.addr_in4).sin_addr.s_addr = inet_addr(pszIpOrDomainName);			
		if((addr.addr_in4).sin_addr.s_addr == (uint32_t)-1)
		{
			struct hostent *host = gethostbyname(pszIpOrDomainName);			
			if (host != NULL && host->h_length > 0) 
			{
				LOG_INFO("get ip by host name = %s",pszIpOrDomainName);
				(addr.addr_in4).sin_addr = *(in_addr*)host->h_addr_list[0];
			}
             else
             {
                LOG_ERROR("unknown sock address = %s ",pszIpOrDomainName);
                return;
             }
		}
	}
    //valid addresss
    addr.addr_in4.sin_family = AF_INET;
}
const char* SockAddress::ToString()
{
    int iType = GetAddressType();
    switch(iType)
    {
        case AF_INET:
        {
            in_addr iaddr;
            iaddr.s_addr = GetIP();
        	snprintf(szSockAddrBuffer,
        					sizeof(szSockAddrBuffer),"%s:%d",
        					inet_ntoa(iaddr),
        					ntohs(GetPort()));	
        }
            break;
        case AF_INET6:
        {
            strcpy(szSockAddrBuffer,"null");
        }
            break;
        case AF_UNIX:
        {
            strncpy(szSockAddrBuffer,
                        addr.addr_un.sun_path,
                        sizeof(szSockAddrBuffer)-1);
        }
            break;

    }
	return szSockAddrBuffer;
}

uint16_t    SockAddress::GetPort() const
{
	return (addr.addr_in4).sin_port;
}
uint32_t    SockAddress::GetIP()   const
{
	return (addr.addr_in4).sin_addr.s_addr;
}
uint16_t    SockAddress::GetAddressType() const
{
    return *(uint16_t*)(&addr);
}
uint32_t    SockAddress::GetSockAddrLen() const
{
    int iType = GetAddressType();
    switch(iType)
    {
        case AF_INET:
            return sizeof(addr.addr_in4);
        case AF_INET6:
            return sizeof(addr.addr_in6);
        case AF_UNIX:
            //offsetof
            return offsetof(struct sockaddr_un,sun_path) + strlen(addr.addr_un.sun_path) + 1;
    }
    //unspecified 
    return 0;
}



//--------------------------------------------
Socket::~Socket()
{
    //copyable , can't close the socket auto when copy.
    //
}     
Socket::Socket(int fd):iFd(fd)
{
}
void	Socket::Close()
{
	if(GetFD() >= 0)
	{
        LOG_INFO("close socket fd = %d",GetFD());
		close(GetFD());
		SetFD(-1);
	}
}
int	Socket::Init()
{
    iFd = -1;
    LOG_ERROR("init default socket fd = -1 ");
    return 0;
}
int	Socket::SetNonBlock(bool bSet)
{	
	return SetFlag(O_NONBLOCK,bSet);
}
int Socket::ReuseAddr(bool bSet)
{
    int v = bSet?1:0;
    return SetOption(SO_REUSEADDR,&v,sizeof(v));
}
int Socket::SetOption(int name,void* pValBuff,int iValBuff)
{
    return    setsockopt(GetFD(), SOL_SOCKET,name,pValBuff,iValBuff);
}

int	Socket::SetFlag(int iFlag,bool bSetOpen)
{
	int iFlags = fcntl(GetFD(), F_GETFL, 0);
	if(bSetOpen)
	{
		iFlags |= iFlag;	
	}
	else
	{
		iFlag  &= (~iFlag);
	}
	return fcntl(GetFD(), F_SETFL, iFlags);	
}
//bind local address
int Socket::Bind(const SockAddress & local)
{
    if(::bind(GetFD(),
        (struct sockaddr *)&(local.addr), 
		local.GetSockAddrLen() ) < 0) 
	{
		LOG_ERROR("bind error !");
		close(GetFD());
        SetFD(-1);
		return -1;
	}
    return 0;
}
int	Socket::ConnectTo(const SockAddress & remote)
{

	int ret = connect(GetFD(),(struct sockaddr*)&remote.addr.addr_in4,
                    (socklen_t)sizeof(remote.addr.addr_in4));
     if(0 == ret)
     {
        return 0;
     }
	if( ret < 0 &&
         errno != EINTR &&
         errno != EWOULDBLOCK &&
         errno != EAGAIN &&
         errno != EINPROGRESS )
	{
		LOG_ERROR("connect error ret = %d errorno = %d!",ret,errno);
		return -1;
	}
     //connecting
	return 1;
}

const 	SockAddress 	Socket::GetLocalAddress()
{
    struct sockaddr addr;
    socklen_t   addrlen;
    int iRet = getsockname(GetFD(),&addr,&addrlen);
    if(iRet < 0)
    {
        LOG_FATAL("get fd = %d local sock name ret = %d < 0 errno = %d",GetFD(),iRet,errno);
        static SockAddress s_empty;
        return s_empty;
    }
    return SockAddress(addr);
}
SockAddress Socket::GetPeerAddress()
{
    struct sockaddr addr;
    socklen_t   addrlen;
    int iRet = getpeername(GetFD(),&addr,&addrlen);
    if(iRet < 0)
    {
        LOG_FATAL("get fd = %d peer name ret = %d < 0 errno = %d",GetFD(),iRet,errno);
        static SockAddress s_empty;
        return s_empty;
    }
    return SockAddress(addr);
}
int      Socket::GetErrorState()
{
    int ret = 0;
    socklen_t   len = 0;
    int iRet = getsockopt(GetFD(),SOL_SOCKET,SO_ERROR,&ret,&len);
    if(iRet != 0)
    {
        return iRet;
    }
    if(iRet == 0 && ret == 0)
    {
        return 0;
    }
    LOG_INFO("get socket opt so_error is not 0 but = %d",ret);
    return ret;
}

int	Socket::Send(const Buffer & sendBuffer, int iFlags /*=0*/)
{
	int iSent = 0;
    int iSentTotal = 0;
	//just send sendBuff.iUsed bytes
	//write <=> send(...,flag=0);
	if(sendBuffer.iUsed <= 0)
	{
        LOG_ERROR("send buffer but used = %d <= 0",sendBuffer.iUsed);        
        return -1;
    }
	do
	{
        iSent = send(GetFD(),sendBuffer.pBuffer + iSentTotal, sendBuffer.iUsed - iSentTotal,iFlags);	
        if(iSent > 0)
        {
            iSentTotal += iSent;
        }
    }while( ((iSent < 0) && (errno == EINTR)) ||
			(iSent > 0 && sendBuffer.iUsed - iSentTotal > 0));			
	if(iSent > 0)
	{
        //LOG_DEBUG("send buffer [END CHAR ASCII=0x%02x] len=%d used=%d !",sendBuffer.pBuffer[iSentTotal-1],iSentTotal,sendBuffer.iUsed);
		return 0;
	}
    else if(0 == iSent)
	{
		//close by peer or error ?
		return 1;
	}
	else if(iSent < 0 &&
	   errno != EINTR &&
	   errno != EAGAIN &&
	   errno != EWOULDBLOCK )
	{
		LOG_ERROR("socket fd = %d error ! ret = %d eno = %d",GetFD(),iSent,errno);
		return -1;
	}
    return 0;
}
//0:ok
//1:close by peer , 2:socket error 
//<0: error
int	Socket::Recv(Buffer& recvBuff,int iFlags /*=0*/) 
{
	recvBuff.iUsed = 0;
    if(recvBuff.iCap <= 0)
    {
        LOG_ERROR("recv but buffer cap = %d <= 0",recvBuff.iCap);
        return -1;
    }
	int iRead = 0;
	do
	{	
		//read <=> recv(...,flag=0);
		iRead = recv(GetFD(),recvBuff.pBuffer + recvBuff.iUsed, recvBuff.iCap - recvBuff.iUsed,iFlags);			
		if(iRead > 0)
		{
			recvBuff.iUsed += iRead;			
		}
	}while( ((iRead < 0) && (errno == EINTR)) ||
			(iRead > 0 && recvBuff.iCap - recvBuff.iUsed > 0));			
	if(iRead > 0)
	{
        //LOG_DEBUG("read buffer [END CHAR ASCII=0x%02x] len=%d cap=%d !",recvBuff.pBuffer[recvBuff.iUsed-1],recvBuff.iUsed,recvBuff.iCap);        
		return 0;
	}
	else if(0 == iRead)
	{
		//close by peer
		return 1;
	}
	else if(iRead < 0 &&
	   errno != EINTR &&
	   errno != EAGAIN &&
	   errno != EWOULDBLOCK )
	{
		LOG_ERROR("socket fd = %d error ! ret = %d eno = %d",GetFD(),iRead,errno);
		return -1;
	}	
	return 0;
}

