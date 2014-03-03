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
 *   Last Changed: 03/03/2014 12时46分46秒
 *
 * =====================================================================================
 */



#pragma once
#include "Socket.h"

//simple tcp socket
class TcpSocket : public Socket
{
public:	
	//return 0 is ok. or fail.
	int 	    Init();
    int      Bind(const SockAddress & local);
	int	    AttachFD(int fd);
	int	    SetNonBlock(bool bSet);	
	int	    SetNagle(bool bSetOpen);
	int	    SetOption(int iFlag,bool bSetOpen);
	int	    Listen(int iBacklog = 128);
    int      GetConnErrorState();
    	SockAddress 	GetPeerAddress();
    //return 0 , connected ok
    //return 1 , connecting
    //< 0:  error
	int	    ConnectTo(const SockAddress & remote);
	TcpSocket  Accept();		
	// return < 0 is error. 0 is ok.
	// it will auto call close.
	int		Send(const Buffer & sendBuffer) ;			
	// return < 0 is error. 0 is ok.
	int		Recv(Buffer& recvBuff) ;	
	void	    Close();
	//-    
public:
	TcpSocket();
    TcpSocket(int fd,const SockAddress & local);
	virtual ~TcpSocket();	
};






