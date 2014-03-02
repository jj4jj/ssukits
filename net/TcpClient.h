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

#pragma once 

#include "TcpSocket.h"
#include "TcpClientHandler.h"
#include "Epoll.h"
//multi thead 
//one client one socket (connection)
class TcpClient
{
public:
    //create socket
    int Init();
    void SetPoller(Epoll*    _pEpoll);
    void SetHandler(TcpClientHandler* pHandler);
    //connect to server
    //poller add its fd
    int     ConnectTo(const SockAddress & remote );    
    //close the clend 
    //poller remove its fd
    void    Close();
    TcpSocket&  GetSocket(){return clientSocket;}
public:
    TcpClient();    
    ~TcpClient();
    void    construct();
    void    destruct();   

    
private:
    TcpSocket           clientSocket;
    Epoll               *pEpoll;    
    TcpClientHandler    *pClientHandler;    
};

 

