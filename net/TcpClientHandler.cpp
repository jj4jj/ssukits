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

void        TcpClientHandler::SetFD2TcpSocketMap(FDTcpSocketMap* pMap)
{
    pMpFD2TcpSocket = pMap;
}
void        TcpClientHandler::SetPoller(Epoll* _pepoll)
{
    pEpoll =  _pepoll;
}

//after conecting
int     TcpClientHandler::OnWritable(int fd)
{
    TcpSocket * pSock =  pMpFD2TcpSocket->GetTcpSocketFromFD(fd);
    if(NULL == pSock)
    {
        LOG_FATAL("fd = %d has no tcp socket map",fd);
        return -1;
    }
    if(pSock->GetErrorState() == 0)
    {
        pEpoll->Mod(pSock->GetFD(),EPOLLIN);
        return OnConnected(*pSock);
    }
    return 0;
}
int     TcpClientHandler::OnReadable(int fd)
{
    TcpSocket * pSock = pMpFD2TcpSocket->GetTcpSocketFromFD(fd);
    if(NULL == pSock)
    {
        LOG_FATAL("fd = %d has no tcp socket map",fd);
        return -1;
    }
    int iRet =  pSock->Recv(_recvBuffer);
    if( 0 ==iRet )
    {
        iRet= OnDataRecv(*pSock,_recvBuffer);
    }
    else if( 1 == iRet ||
             iRet < 0 )
    {
         iRet =  OnDisconnected(*pSock);
         pSock->Close();           
         pMpFD2TcpSocket->RemoveFDTcpSocket(fd);
    }
    else        
    {
        LOG_ERROR("client read return val =  %d",iRet);
    }
    return iRet;
}   

 

