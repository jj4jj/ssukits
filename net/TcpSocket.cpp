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
 *   Last Changed: 05/22/2014 11时28分02秒
 *
 * =====================================================================================
 */



#pragma once
#include "Socket.h"
#include "base/Log.h"
#include "base/CommonMacro.h"
#if 1
TcpSocket::TcpSocket()
{
	SetConnFlag(CONN_FLAG_ALL,false);
	SetFD(-1);
	//default handler.
	pDefHandler = NULL;
    pParentSocket = NULL;
}
TcpSocket::~TcpSocket()
{
	SAFE_DELETE(pDefHandler);
    pParentSocket = NULL;

    //--------must be at last---------
	Close();
}
#endif


#if 1
//return 0 is ok. or fail.
int 	TcpSocket::Init(const SockAddress & local,TcpConnHandler* pHandler_ = NULL)
{
	//create socket , then set socket handler	
	//ipv4
	int fd = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if( fd < 0)
	{
		LOG_ERROR("call socket error !");
		return -1;
	}
	int iReuseAddr = 1;
	setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &iReuseAddr, 
		sizeof(iReuseAddr));
	SetNonBlock(true);
	SetNagle(true);
	if(bind(fd, (struct sockaddr *)(local.sock_addr), 
		sizeof(struct sockaddr_in)) < 0) 
	{
		LOG_ERROR("bind error !");
		close(fd);
		return -1;
	}		
	SetLoacalAddress(local);
	if(NULL == pHandler_)
	{
		pDefHandler = new TcpConnHandler();	
		pHandler_ = pDefHandler;
	}	
	SetFD(fd);
	SetHandler(pHandler_);	
	return 0;
}
int  TcpSocket::AttachFD(int fd)
{
    if(fd < 0)
    {
        LOG_ERROR("attach fd = %d error !",fd);
        return -1;
    }
    SetFD(fd);    
}
void TcpSocket::SetHandler(TcpConnHandler* pHandler_)
{
	pHandler = pHandler_;	
	pHandler->SetHostSocket(this);
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
	int iFlags = fcntl(iSock, F_GETFL, 0);
	if(bSetOpen)
	{
		iFlags |= iFlag;	
	}
	else
	{
		iFlag  &= (~iFlag);
	}
	return fcntl(iSock, F_SETFL, iFlags);	
}
int	TcpSocket::Listen(int iBacklog)
{
	if(listen(GetFD(),iBacklog) < 0)
	{
		LOG_ERROR("listen error !");
		return -1;
	}
	//-----------------------------------
	SetConnFlag(CONN_FLAG_LISTENING,true);
	return 0;
}
int	TcpSocket::ConnectTo(const SockAddress & remote)
{
	int ret = connect(GetFD(),remote.sock_addr,sizeof(remote.sock_addr));
     if(0 == ret)
     {
        	SetPeerAddress(remote);
        pHandler->OnConnected();
        return 0;
     }
	if( ret < 0 ||
         ret != EINTR &&
         ret != EWOULDBLOCK &&
         ret != EAGAIN )
	{
		LOG_ERROR("connect error ret = %d errorno = %d!",ret,errno);
		return -1;
	}
    
    	SetPeerAddress(remote);
	//------------------------------------
	SetConnFlag(CONN_FLAG_CONNECTING,true);
	return 0;
}
void    TcpSocket::SetParentSocket(TcpSocket* parent_)
{
    pParentSocket = parent_;
}
TcpSocket*  TcpSocket::NewAcceptSocket(int fd,const SockAddress& local,const SockAddress & remote)
{
    TcpSocket * pNewSock = new TcpSocket();
    if(NULL == pNewSock)
    {
        LOG_ERROR("new tcpsocket error !");
        return NULL;
    }

    pNewSock->AttachFD(fd);
    pNewSock->SetLocalAddress(local);
    pNewSock->SetPeerAddress(remote);
    pNewSock->SetParentSocket(this);
    
    pNewSock->SetConnFlag(TcpSocket::CONN_FLAG_ESTABLISHED);      
    return pNewSock;
}
void  TcpSocket::DeleteAcceptSocket(TcpSocket* pSocket)
{
    SAFE_DELETE(pSocket);
}


TcpSocket*  TcpSocket::Accept()
{

     struct sockaddr    addr;
     socklen_t          addrlen = sizeof(struct sockaddr);
     int ifd = accept(GetFD(),&addr,&addrlen);
     if(fd < 0)
     {
        LOG_ERROR("socket fd = %d accept error no = %d",GetFD(),errno);
        return NULL;
     }

     TcpSocket* pNewSock = NewAcceptSocket(ifd,GetLocalAddress(),SockAddress(addr));
     if(NULL == pNewSock)
     {
        return NULL;
     }
     return pNewSock;    
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
         pHandler->OnDataSent(sendBuffer);        
		return 0;
	}
    else if(0 == iSent)
	{
		//close by peer
		Close(false);
		return -1;
	}
	else if(iSent < 0 &&
	   iSent != EINTR &&
	   iSent != EAGAIN &&
	   iSent != EWOULDBLOCK )
	{
		LOG_ERROR("socket fd = %d error ! ret = %d eno = %d",GetFD(),iSent,errno);
		Close();
		return -2;
	}
    return 0;
}
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
         pHandler->OnDataRecv(recvBuff);
		return 0;
	}
	else if(0 == iRead)
	{
		//close by peer
		Close(false);
		return -1;
	}
	else if(iRead < 0 &&
	   iRead != EINTR &&
	   iRead != EAGAIN &&
	   iRead != EWOULDBLOCK )
	{
		LOG_ERROR("socket fd = %d error ! ret = %d eno = %d",GetFD(),iRead,errno);
		Close();
		return -2;
	}	
	return 0;
}
void	TcpSocket::Close(bool bBySelf)
{
	if(GetFD() >= 0)
	{
		close(GetFD());
		SetFD(-1);
	}
	SetConnFlag(CONN_FLAG_ALL,false);
	pHandler->OnClosed(bBySelf);
    LOG_INFO("close socket by self = %d",bBySelf?1:0);




    //---------------------------must be at last-----------------
    if(pParentSocket!=NULL)
    {
        pParentSocket->DeleteAcceptSocket(this);    
    }
}
#endif

#if 1
void	TcpSocket::SetConnFlag(TcpConnectionStateFlag bFlag,bool bSet)
{
	if(0 == bFlag && !bSet)
	{
		bConnFlag = 0;
	}
	else
	{
		bFlag |= bFlag;
	}
}
bool	TcpSocket::GetConnFlag(TcpConnectionStateFlag bFlag)
{
	if(0 != bFlag)
	{
		return bConnFlag&bFlag;
	}
}
#endif
