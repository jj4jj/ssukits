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


#include "TcpServer.h"
#include "base/Log.h"
///////////////////////////////////////////////////////////////////////////////////


int 	TcpServer::Init(const SockAddress & localAddr,bool bOpenNagle,int iMaxConnxNum )
{
    //crate socket and epoll
    if(listenSocket.Init())
    {
        return -1;
    }
    if(listenSocket.Bind(localAddr))
    {
        return -1;
    }
    listenSocket.SetNagle(bOpenNagle);
    if(epoll.Init(iMaxConnxNum))
    {
        return -1;
    }
    LOG_INFO("server listen on :%s ok nagle:%d max conn num = %d",
             localAddr.ToString(),bOpenNagle,iMaxConnxNum);
    return 0;
}
int	TcpServer::Start()
{
    if(pServerHandler == NULL)
    {
        LOG_FATAL("there has no server handler !");
        return -1;
    }
    //add listen socket to epoll
    if(epoll.Add(listenSocket.GetFD(),EPOLLIN))
    {            
        return -1;
    }
    if(listenSocket.Listen())
    {
        return -1;
    }  
    bNeedRunning = true;
    return 0;        
}

void  TcpServer::ShutDown()
{
    bNeedRunning = false;
}
int	TcpServer::Loop(int iProcessNum )
{
    if(iProcessNumThisRound >= iNfds)
    {
        iProcessNumThisRound = 0;
        if(!bNeedRunning)
        {
            return -1;
        }
        iNfds = epoll.Polling(&pEvents);
        if(iNfds < 0 ||
           NULL == pEvents)
        {
            return -1;
        }            
    }
    for(int i = 0; i < iProcessNum &&
                iProcessNumThisRound < iNfds ;
                ++i,++iProcessNumThisRound)
    {
        //just focus epollin event.
        if(pEvents[iProcessNumThisRound].events & EPOLLIN)
        {
            
            int efd = pEvents[iProcessNumThisRound].data.fd;
            if(listenSocket.GetFD() == efd)
            {
                pServerHandler->OnAcceptable();
            }
            else
            {
                pServerHandler->OnClientReadable(efd);
            }
        }            
    }
    return 0;        
}

void  TcpServer::SetServerHandler(TcpServerHandler* pHandler)
{
    pServerHandler = pHandler;
    pServerHandler->SetHostSocket(&listenSocket);
    pServerHandler->SetHostPoller(&epoll);
}   
TcpSocket& TcpServer::GetListenSocket()
{
    return listenSocket;
}

void TcpServer::construct()
{
    iProcessNumThisRound = 0;
   bNeedRunning = false;
   iNfds = 0;
   pEvents = NULL;
   pServerHandler = NULL;
}
void TcpServer::destruct()
{
    epoll.Destroy();
    listenSocket.Close();    
    iProcessNumThisRound = 0;
   bNeedRunning = false;
   iNfds = 0;
   pEvents = NULL;
   pServerHandler = NULL;   
}
TcpServer::TcpServer()
{
    construct();
}
TcpServer::~TcpServer()
{
    destruct();
}





