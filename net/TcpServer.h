/*
 * =====================================================================================
 *
 *       Filename:  TcpServer.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2014年05月22日 05时00分07秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  sean
 *   Organization:  
 *   Last Changed: 05/22/2014 12时20分44秒
 *
 * =====================================================================================
 */


#pragma once

#include "TcpSocket.h"
#include "TcpConnPoller.h"
#include "TcpConnHandler.h"


//polling policy will be extened in future.
//default is use epoll
template <	class TcpConnHandlerType = TcpConnHandler ,
			class TcpConnPollerType  = TcpConnEpoll >
class TcpServer
{
public:
	int 	Init(const SockAddress & localAddr,bool bAsyncSocket = true,
			bool bOpenNagle = false,
			int  iMaxConnNum = 1024 );	
	TcpSocket&	GetListenSocket(){return listenSocket;}
	int	Start();
	int	Polling(int iProcessNum = 1024);
public:
	TcpServer():iProcessNumThisRound(0)
	{
		events.clear();
	}
private:
	TcpSocket			listenSocket;	
	TcpConnPollerType	poller;
	TcpConnHandlerType	serverHandler;
	std::vector<TcpConnPollerEvent> & events;
	int					iProcessNumThisRound;
};



//---------------------------------------------------------------------------
int 	TcpServer::Init(const SockAddress & localAddr,bool bAsyncSocket = true,
		bool bOpenNagle = false,
		int  iMaxConnNum = 1024 )
{
	if(listenSocket.Init(lockAddr) != 0)
	{
		return -1;
	}
	listenSocket.SetNonBlock(bAsyncSocket);
	if(poller.Init(iMaxConnNum) != 0)
	{
		return -1;
	}
	serverHandler.SetHostSocket(&listenSocket);
	serverHandler.SetHostPoller(&poller);
	return 0;
}
int	TcpServer::Start()
{
	if(listenSocket.Listen() != 0)
	{
		return -1;
	}
	if(poller.Add(&listenSocket,TcpConnPollerEvent::ET_ACCEPT) != 0)
	{
		return -1;
	}
	return 0;
}
int	TcpServer::Polling(int iProcessNum = 1024)
{
	if(iProcessNumThisRound >= events.size())
	{
		int iNfds = poller.Polling(events);
		iProcessNumThisRound = 0;
		if(iNfds <= 0)
		{
			return iNfds;
		}	
	}
	for(int i = iProcessNumThisRound;
		i < events.size();
		++i,++iProcessNumThisRound)
	{
		TcpSocket* pSocket = poller.GetTcpSocketFromEvent(events[i]);
		if(NULL == pSocket)
		{
			LOG_FATAL("get socket error !");
			continue;
		}
		TcpConnHandler* pHandler = pSocket->GetHandler();
		switch(events[i].bEventType)
		{
			case ET_ACCEPT:
			pHandler->OnAcceptable();
			break;
			case ET_READ:
			pHandler->OnRecvable();
			break;
			case ET_WRITE:
			pHandler->OnSendable();
			break;
			case ET_CONNECT:
			pHandler->OnConnected();
			break;
		}
	}
	return iProcessNumThisRound;
}
//---------------------------------------------------------------------
