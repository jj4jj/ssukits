#include "UdpSocket.h"
#include "base/Log.h"

int     UdpSocket::Init()
{
    if(GetFD() >= 0)
    {
        LOG_ERROR("init a updsocket repeat ! fd = %d",GetFD());
        return -1;
    }
    int fd = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
    if(fd < 0)
    {
        LOG_ERROR("call socket error !");
        return -1;
    }
    SetFD(fd); 
    SetNonBlock(true);
    ReuseAddr(true);
    return 0;
}

int		UdpSocket::SendTo(const Buffer & sendBuffer,const SockAddress & dstAddr,int iFlags )
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
        iSent = sendto(GetFD(),sendBuffer.pBuffer + iSentTotal,sendBuffer.iUsed - iSentTotal,
                iFlags,(struct sockaddr*)&(dstAddr.addr.addr_in4),sizeof(dstAddr.addr.addr_in4));
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
int		UdpSocket::RecvFrom(Buffer& recvBuff ,SockAddress & srcAddr, int iFlags )
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
        socklen_t slen = sizeof(srcAddr.addr.addr_in4);
		iRead = recvfrom(GetFD(),recvBuff.pBuffer + recvBuff.iUsed,
                recvBuff.iCap - recvBuff.iUsed,iFlags,(struct sockaddr*)&(srcAddr.addr.addr_in4),&slen);          
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




