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
	int 	Init();
	int	    AttachFD(int fd);
	int	    SetNagle(bool bSetOpen);
	TcpSocket  Accept();		
	int	    Listen(int iBacklog = 128);
public:
    //default is a error socket . uninited
    TcpSocket(int fd = -1 );
};






