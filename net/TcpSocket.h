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


//simple tcp socket
class TcpSocket : public Socket
{
	
	//return 0 is ok. or fail.
	int 	Init(const SockAddress & local,TcpConnHandler* pHandler = NULL);
	int		AttachFD(int fd);
	void 	SetHandler(TcpConnHandler* pHandler);
	int	SetNonBlock(bool bSet);	
	int	SetNagle(bool bSetOpen);
	int	SetOption(int iFlag,bool bSetOpen);
	int	Listen(int iBacklog);
	int	ConnectTo(const SockAddress & remote);
	TcpSocket * Accept();		
	// return < 0 is error. 0 is ok.
	// it will auto call close.
	int		Send(const Buffer & sendBuffer) ;			
	// return < 0 is error. 0 is ok.
	int		Recv(Buffer& recvBuff) ;	
	void	    Close();
	//-
	bool    	IsConnected(){return GetConnFlag(CONN_FLAG_ESTABLISHED);}
	bool 	IsConnecting(){return GetConnFlag(CONN_FLAG_CONNECTING);}
	bool 	IsListening(){return GetConnFlag(CONN_FLAG_LISTENING);}
public:
    void        SetParentSocket(TcpSocket* parent_);
    TcpSocket*  NewAcceptSocket(int fd,const SockAddress& local,const SockAddress & remote);
    void        DeleteAcceptSocket(TcpSocket* pSocket);
    
private:
	void	SetConnFlag(TcpConnectionStateFlag bFlag,bool bSet);
	bool	GetConnFlag(TcpConnectionStateFlag bFlag);
public:
	TcpSocket();
	~TcpSocket();
	//action flaging;
	enum TcpConnectionStateFlag
	{
		CONN_FLAG_ALL = 0,	  //clear all
		CONN_FLAG_CONNECTING = 1, //use client
		CONN_FLAG_LISTENING =  2, //use server
		CONN_FLAG_ESTABLISHED = 4,//use client
	};
	//handler should call it .
	void	OnConneted()
	{
		SetConnFlag(CONN_FLAG_CONNECTING,false);
		SetConnFlag(CONN_FLAG_ESTABLISHED,true);
	}
	void	OnDisconnnected()
	{
		SetConnFlag(CONN_FLAG_ESTABLISHED,false);
	}
	
	
private:
	uint8_t	bConnFlag;
	TcpConnHandler *	pHandler;
	TcpConnHandler *	pDefHandler;	
    TcpSocket*        pParentSocket;
};
