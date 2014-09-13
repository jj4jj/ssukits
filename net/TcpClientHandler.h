
#pragma once

#include "TcpSocket.h"
#include "Epoll.h"
#include "TcpSocketHelper.h"

class TcpClientHandler
{
public:
    //call back , must be implemtn
    //when tcp.connected ok or connecting when writable
    virtual     int     OnConnected( TcpSocket &   client) = 0;
    //readable
    virtual     int     OnDataRecv( TcpSocket &   client,const Buffer & recvBuffer) = 0;
    //peer close .
    virtual     int     OnDisconnected( TcpSocket &   client) = 0;

public:
    void        SetPoller(Epoll* _pEpoll);
    enum
    {
        DEAFAULT_MAX_RECV_BUFFER_SIZE = 65536,
    };
    TcpClientHandler();
    virtual    ~TcpClientHandler();
    //after conecting
    virtual     int     OnWritable(int fd);
    virtual     int     OnReadable(int fd); 
private:
    Epoll               *pEpoll;        
    Buffer              _recvBuffer;    
};

typedef shared_ptr<TcpClientHandler>    TcpClientHandlerSharedPtr;





















