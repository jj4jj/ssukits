#include "TcpClientHandler.h"
#include "base/Log.h"
///////////////////////////////////////////////////////////////////////////////////

TcpClientHandler::TcpClientHandler()
{
    pEpoll = NULL;
    _recvBuffer.Create(DEAFAULT_MAX_RECV_BUFFER_SIZE);
}
TcpClientHandler::~TcpClientHandler()
{
    _recvBuffer.Destroy();
}        

void        TcpClientHandler::SetPoller(Epoll* _pepoll)
{
    pEpoll =  _pepoll;
}

//after conecting
int     TcpClientHandler::OnWritable(int fd)
{
    TcpSocket sock(fd);
    if(sock.GetErrorState() == 0)
    {
        pEpoll->Mod(fd,EPOLLIN);
        return OnConnected(sock);
    }
    return 0;
}
int     TcpClientHandler::OnReadable(int fd)
{
    TcpSocket sock(fd);
    int iRet =  sock.Recv(_recvBuffer);
    if( 0 ==iRet )
    {
        iRet= OnDataRecv(sock,_recvBuffer);
    }
    else if( 1 == iRet ||
             iRet < 0 )
    {
         OnDisconnected(sock);
         sock.Close();      
         return -1;
    }
    else        
    {
        LOG_INFO("client read return val =  %d",iRet);
    }
    return iRet;
}   

 

