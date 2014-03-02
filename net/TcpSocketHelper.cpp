#include "TcpSocketHelper.h"
#include "base/Log.h"
TcpSocket* FDTcpSocketMap::GetTcpSocketFromFD(int fd)
{
    FD2TcpSocketMapItr   it = mpFD2TcpSocket.find(fd);
    if(it != mpFD2TcpSocket.end())
    {
        return &(it->second);
    }
    return NULL;
}   
int        FDTcpSocketMap::AddFDTcpSocket(int fd,const TcpSocket & sock)
{
    FD2TcpSocketMapItr   it = mpFD2TcpSocket.find(fd);
    if(it != mpFD2TcpSocket.end())
    {
        LOG_FATAL("add fd = %d  tcp socket map repeat !",fd);
        return -1;
    }
    mpFD2TcpSocket[fd] = sock; 
    return 0;        
}
int        FDTcpSocketMap::RemoveFDTcpSocket(int fd)
{
    mpFD2TcpSocket.erase(fd);
    return 0;
}

