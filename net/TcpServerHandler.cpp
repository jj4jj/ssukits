
#include "TcpServerHandler.h"
#include "base/Log.h"
///////////////////////////////////////////////////////////////////////////////////
void        TcpServerHandler::SetHostSocket(TcpSocket* pHost)
{
    pHostSocket = pHost;
}
void        TcpServerHandler::SetHostPoller(Epoll*     pPoller)
{
    pEpoll = pPoller;
}
TcpServerHandler::TcpServerHandler()
{
    pHostSocket = NULL;
    pEpoll = NULL;
    _recvBuffer.Create(DEFAULT_RECV_BUFFER_SIZE);
}
TcpServerHandler::~TcpServerHandler()
{
    pHostSocket = NULL;
    pEpoll = NULL;
    _recvBuffer.Destroy();       
}
//dispatcher
int     TcpServerHandler::OnAcceptable()
{
    TcpSocket client = pHostSocket->Accept();
    if(client.GetFD() < 0)
    {
        return -1;
    }        
    client.SetNonBlock(true);
    client.SetNagle(true);    
    mpFD2TcpSocket.AddFDTcpSocket(client.GetFD(),client);
    //just consider in read event
    pEpoll->Add(client.GetFD(),EPOLLIN);
    return OnNewConnection(client);        
}
int     TcpServerHandler::OnClientReadable(int fd)
{
    // < 0 .error
    // = 0 .ok
    // = 1 . peer close
    TcpSocket* pClient = mpFD2TcpSocket.GetTcpSocketFromFD(fd);
    if(NULL == pClient)
    {
        LOG_FATAL("client fd = %d is not exist socket object",fd);
        return -1;
    }
    TcpSocket& client = *pClient;
    int iRet = client.Recv(_recvBuffer);
    if(0 == iRet)
    {
        iRet = OnClientDataRecv(client,_recvBuffer);            
    }
    else if( 1 == iRet ||
            iRet < 0)
    {            
        int clifd = client.GetFD();
        iRet = OnConnectionClosed(client);            
        pEpoll->Del(clifd);               
        client.Close();            
        mpFD2TcpSocket.RemoveFDTcpSocket(clifd);
    }
    else
    {
        LOG_ERROR("client read return val =  %d",iRet);
    }
    return  iRet;
}
   
