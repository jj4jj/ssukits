
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
    //just consider in read event
    pEpoll->Add(client.GetFD(),EPOLLIN);
    return OnNewConnection(client);        
}
int     TcpServerHandler::OnClientReadable(int fd)
{
    // < 0 .error
    // = 0 .ok
    // = 1 . peer close
    TcpSocket sock(fd);
    TcpSocket* pClient = &sock;
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
        //pEpoll->Del(clifd);//when fd close , system will auto remove epoll fd               
        client.Close();            
    }
    else
    {
        LOG_ERROR("client read return val =  %d",iRet);
    }
    return  iRet;
}
   
