#pragma once
#include "net/TcpServerHandler.h"
#include "net/TcpClientHandler.h"
#include "net/UdpSocketHandler.h"

class PingPangServerHandler: public TcpServerHandler
{
public:
    int     OnNewConnection( TcpSocket   &   client);
    int     OnClientDataRecv( TcpSocket &   client,const Buffer & recvBuffer);
    int     OnConnectionClosed( TcpSocket &  client);
    ~PingPangServerHandler(){}
    PingPangServerHandler(){iClientIdx = 0;iClientCount = 0;lRecvTotal = 0;}
private: 
    int iClientIdx; 
    int iClientCount;    
    long    lRecvTotal;
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


class PingPangUdpHandler: public UdpSocketHandler
{
public:    
    int OnDataRecv(UdpSocket& udpSock,const Buffer & recvBuffer,const SockAddress & addr);
};



