#pragma once

#include "Socket.h"

class UdpSocket : public Socket
{
public:
    //init a socket with udp option.
    int     Init();
    //recv from
	int		SendTo(const Buffer & sendBuffer,const SockAddress & dstAddr,int iFlags = 0);			
	int		RecvFrom(Buffer& recvBuff ,SockAddress & dstAddr, int iFlags = 0);	    
public:
	UdpSocket(int fd=-1):Socket(fd){}
};







