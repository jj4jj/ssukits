/*
 * =====================================================================================
 *
 *       Filename:  TcpSocket.h
 *
 *    Description:  sean simple net lib
 *
 *        Version:  1.0
 *        Created:  2014年05月22日 04时59分46秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  sean
 *   Organization:  
 *   Last Changed: 03/03/2014 12时46分29秒
 *
 * =====================================================================================
 */

#include "TcpSocket.h"
#include "base/Log.h"
#include "base/CommonMacro.h"

#if 1
TcpSocket::TcpSocket()
{
	SetFD(-1);
}
TcpSocket::TcpSocket(int fd,const SockAddress & local)
{
    SetFD(fd);    
    SetLocalAddress(local);
}
TcpSocket::~TcpSocket()
{
    
}
#endif


#if 1


int TcpSocket::Bind(const SockAddress & local)
{
    if(::bind(GetFD(),
        (struct sockaddr *)&(local.sock_addr), 
		(socklen_t)sizeof(struct sockaddr) ) < 0) 
	{
		LOG_ERROR("bind error !");
		close(GetFD());
         SetFD(-1);
		return -1;
	}
	SetLocalAddress(local);
    return 0;
}
//return 0 is ok. or fail.
int 	TcpSocket::Init()
{
	//create socket , then set socket handler	
	//ipv4
	int fd = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if( fd < 0)
	{
		LOG_ERROR("call socket error !");
		return -1;
	}
	SetFD(fd);    
	int iReuseAddr = 1;
	setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &iReuseAddr, 
		sizeof(iReuseAddr));
	SetNonBlock(true);
	SetNagle(true);
			
	return 0;
}
int	TcpSocket::SetNonBlock(bool bSet)
{	
	return SetOption(O_NONBLOCK,bSet);
}
int	TcpSocket::SetNagle(bool bSetOpen)
{
	return SetOption(O_NDELAY,bSetOpen);
}
int	TcpSocket::SetOption(int iFlag,bool bSetOpen)
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
int	TcpSocket::Listen(int iBacklog)
{
	if(listen(GetFD(),iBacklog) < 0)
	{
		LOG_ERROR("listen error !");
		return -1;
	}
	return 0;
}
int	TcpSocket::ConnectTo(const SockAddress & remote)
{

	int ret = connect(GetFD(),(struct sockaddr*)&remote.sock_addr,
                    (socklen_t)sizeof(remote.sock_addr));
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
SockAddress TcpSocket::GetPeerAddress()
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
int      TcpSocket::GetConnErrorState()
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
TcpSocket  TcpSocket::Accept()
{
     struct sockaddr    addr;
     socklen_t          addrlen = sizeof(struct sockaddr);
     int ifd = accept(GetFD(),&addr,&addrlen);
     if(ifd < 0)
     {
        LOG_ERROR("socket fd = %d accept error no = %d",GetFD(),errno);
     }
     return TcpSocket(ifd,GetLocalAddress());
}


// return 0 is ok. otherwise is fail.
int	TcpSocket::Send(const Buffer & sendBuffer)
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
        iSent = write(GetFD(),sendBuffer.pBuffer + iSentTotal, sendBuffer.iUsed - iSentTotal);	
        if(iSent > 0)
        {
            iSentTotal += iSent;
        }
    }while( ((iSent < 0) && (errno == EINTR)) ||
			(iSent > 0 && sendBuffer.iUsed - iSentTotal > 0));			
	if(iSent > 0)
	{
         LOG_DEBUG("send buffer [END CHAR ASCII=0x%02x] len=%d used=%d !",sendBuffer.pBuffer[iSentTotal-1],iSentTotal,sendBuffer.iUsed);
		return 0;
	}
    else if(0 == iSent)
	{
		//close by peer
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
int	TcpSocket::Recv(Buffer& recvBuff) 
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
		iRead = read(GetFD(),recvBuff.pBuffer + recvBuff.iUsed, recvBuff.iCap - recvBuff.iUsed);			
		if(iRead > 0)
		{
			recvBuff.iUsed += iRead;			
		}
	}while( ((iRead < 0) && (errno == EINTR)) ||
			(iRead > 0 && recvBuff.iCap - recvBuff.iUsed > 0));			
	if(iRead > 0)
	{
         LOG_DEBUG("read buffer [END CHAR ASCII=0x%02x] len=%d cap=%d !",recvBuff.pBuffer[recvBuff.iUsed-1],recvBuff.iUsed,recvBuff.iCap);        
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
void	TcpSocket::Close()
{
	if(GetFD() >= 0)
	{
        LOG_INFO("close socket fd = %d",GetFD());
		close(GetFD());
		SetFD(-1);
	}
}
#endif

