#pragma once
#include "net/TcpServerHandler.h"
#include "net/TcpClientHandler.h"

class PingPangServerHandler: public TcpServerHandler
{
public:
    int     OnNewConnection( TcpSocket   &   client);
    int     OnClientDataRecv( TcpSocket &   client,const Buffer & recvBuffer);
    int     OnConnectionClosed( TcpSocket &  client);
    ~PingPangServerHandler(){}
    PingPangServerHandler(){iClientIdx = 0;iClientCount = 0;}
private: 
    int iClientIdx; 
    int iClientCount;    
}; 

class PingPangClientHandler: public TcpClientHandler
{
public:    
    int     OnConnected( TcpSocket &   client);
    //readable
    int     OnDataRecv( TcpSocket &   client,const Buffer & recvBuffer);
    //peer close .
    int     OnDisconnected( TcpSocket &   client);
    ~PingPangClientHandler(){}
};


