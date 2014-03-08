/*
 * =====================================================================================
 *
 *       Filename:  TcpClient.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2014年05月22日 05时01分10秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *   Last Changed: 05/22/2014 05时01分13秒
 *
 * =====================================================================================
 */

#include "TcpClient.h"
#include "base/Log.h"


void TcpClient::SetPoller(Epoll*    _pEpoll)
{
    pEpoll = _pEpoll;
}
void TcpClient::SetHandler(TcpClientHandler* pHandler)
{
}

int     TcpClient::Init()
{
    if(clientSocket.Init())
    {
        return -1;
    }
    return 0;
}

int     TcpClient::ConnectTo(const SockAddress & addr)
{
    pEpoll->Add(clientSocket.GetFD(),EPOLLIN|EPOLLOUT);
    int iRet =  clientSocket.ConnectTo(addr);
    if(iRet < 0)
    {
        return -1;
    }
    if(0 == iRet)
    {
        LOG_INFO("client fd = %d is connected ! ",clientSocket.GetFD());
    }
    else if(1 == iRet)
    {
        LOG_INFO("client fd = %d is connecting ! ",clientSocket.GetFD());
    }
    return 0;
}

void    TcpClient::Close()
{

    if(pEpoll)
    {
        pEpoll->Del(clientSocket.GetFD());
    }
    clientSocket.Close();
    destruct();
}

TcpClient::TcpClient()
{
    construct();
}
    
TcpClient::~TcpClient()
{
    destruct();
}

void    TcpClient::construct()
{
    pEpoll = NULL;
}

void    TcpClient::destruct()
{    
    pEpoll = NULL;
}
    

