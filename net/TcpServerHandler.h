#pragma once

#include "TcpSocket.h"
#include "TcpSocketHelper.h"
#include "Epoll.h"
//interface
class TcpServerHandler
{

public:    
    //call back
    virtual     int     OnNewConnection(TcpSocket   &   client) = 0;
    virtual     int     OnClientDataRecv(TcpSocket &   client,const Buffer & recvBuffer) = 0;
    virtual     int     OnConnectionClosed(TcpSocket &  client) = 0;
public:
    TcpServerHandler();
    virtual     ~TcpServerHandler();
    void        SetHostSocket(TcpSocket* pHost);
    void        SetHostPoller(Epoll*     pPoller);
    //dispatcher
    virtual     int     OnAcceptable();
    virtual     int     OnClientReadable(int fd);
private:
    TcpSocket           *pHostSocket;
    Epoll               *pEpoll;
    enum
    {
        //64K
        DEFAULT_RECV_BUFFER_SIZE = 65536,
    };
    Buffer              _recvBuffer;
    FDTcpSocketMap      mpFD2TcpSocket;
};

