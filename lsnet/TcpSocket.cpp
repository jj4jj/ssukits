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
	enum TcpConnectionStateFlag
	{
		CONN_FLAG_ALL = 0,	  //clear all
		CONN_FLAG_CONNECTING = 1, //use client
		CONN_FLAG_LISTENING =  2, //use server
		CONN_FLAG_ESTABLISHED = 4,//use client
	};	

	TcpSocket()
	{
		SetConnFlag(CONN_FLAG_ALL,false);
		SetFD(-1);
	}
	~TcpSocket()
	{
		Close();
	}
	//return 0 is ok. or fail.
	int 	Init(const SockAddress & local);
	int	SetNonBlock(bool bSet);
	int	SetNagle(bool bSetOpen);
	int	SetOption(int iCmd,long lArg);
	int	Listen(int iBacklog)
	{

	
		//-----------------------------------
		SetConnFlag(CONN_FLAG_LISTENING,true);
	}
	int	ConnectTo(const SockAddress & remote)
	{

		//------------------------------------
		SetConnFlag(CONN_FLAG_CONNECTING,true);
	}
	TcpSocket & Accept();	
	

	//action flaging;
	void	OnConneted()
	{
		SetConnFlag(CONN_FLAG_CONNECTING,false);
		SetConnFlag(CONN_FLAG_ESTABLISHED,true);
	}
	void	OnDisconnnected()
	{
		SetConnFlag(CONN_FLAG_ESTABLISHED,false);
	}
	bool	IsConnected(){return GetConnFlag(CONN_FLAG_ESTABLISHED);}
	bool 	IsConnecting(){return GetConnFlag(CONN_FLAG_CONNECTING);}
	bool 	IsListening(){return GetConnFlag(CONN_FLAG_LISTENING);}
	

	// return 0 is ok. otherwise is fail.
	int	Send(const Buffer & sendBuffer) ;			
	int	Recv(Buffer& recvBuff) ;
	void	Close()
	{
		if(GetFD() >= 0)
		{
			close(iFD);
			SetFD(-1);
		}
		SetConnFlag(CONN_FLAG_ALL,false);	
	}
	void	SetConnFlag(TcpConnectionStateFlag bFlag,bool bSet)
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
	bool	GetConnFlag(TcpConnectionStateFlag bFlag)
	{
		if(0 != bFlag)
		{
			return bConnFlag&bFlag;
		}
	}
private:
	uint8_t	bConnFlag;
};
