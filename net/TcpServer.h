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
#include "TcpServerHandler.h"
#include "Epoll.h"



class TcpServer
{
public:
//---------------------------------------------------------------------------
    int 	Init(const SockAddress & localAddr,bool bOpenNagle = true,int iMaxConnxNum = 1024);
    void  SetServerHandler(TcpServerHandler* pHandler);   
    TcpSocket& GetListenSocket();
    int	Start();
    void  ShutDown();
    int	Loop(int iProcessNum = 1024);
public:
    void construct();
    void destruct();
	TcpServer();
    ~TcpServer();
private:
	TcpSocket		    listenSocket;	
	int				    iProcessNumThisRound;
    bool                  bNeedRunning;
    int                   iNfds;
    epoll_event*          pEvents;
    Epoll                 epoll;
    TcpServerHandler*     pServerHandler;
};




