

#pragma once

#include "TcpSocket.h"

//fd 2 socket map
class FDTcpSocketMap
{
public:
    typedef std::map<int,TcpSocket>      FD2TcpSocketMap;    
    typedef FD2TcpSocketMap::iterator    FD2TcpSocketMapItr;
private:
    FD2TcpSocketMap            mpFD2TcpSocket;
public:
    TcpSocket* GetTcpSocketFromFD(int fd);
    int        AddFDTcpSocket(int fd,const TcpSocket & sock);
    int        RemoveFDTcpSocket(int fd);
};



