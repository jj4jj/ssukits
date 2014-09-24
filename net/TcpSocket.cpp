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
TcpSocket::TcpSocket(int fd)
{
    SetFD(fd);   
}
#endif


#if 1
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
	ReuseAddr(true);
	SetNonBlock(true);
	SetNagle(true);
			
	return 0;
}
int	TcpSocket::SetNagle(bool bSetOpen)
{
	return SetFlag(O_NDELAY,bSetOpen);
}
int TcpSocket::SetLinger(struct linger& lgr)
{
    //if ligr on off is 0 ��close will return right now 
    //and send any buffer unsent
    //else if on off is 1 .
    //it will wait linger time , during the linger time
    //kernel send buffer unsent
    //if linger time is 0
    //close will send a RST to peer tcp will be closed state.
    
    return SetOption(SO_LINGER,&lgr,sizeof(struct linger));
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

TcpSocket  TcpSocket::Accept()
{
     struct sockaddr    addr;
     socklen_t          addrlen = sizeof(struct sockaddr);
     int ifd = accept(GetFD(),&addr,&addrlen);
     if(ifd < 0)
     {
        LOG_ERROR("socket fd = %d accept error no = %d",GetFD(),errno);        
     }
     return TcpSocket(ifd);
}
#endif

