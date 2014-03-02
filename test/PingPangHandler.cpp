
#include "PingPangHandler.h"
#include "base/Log.h"
////////////////////////////////////////////////////////////
int     PingPangServerHandler::OnNewConnection(TcpSocket   &   client)
{   
    iClientCount++;
    LOG_INFO("new connection comming fd = %d idx = %d total = %d",client.GetFD(),iClientIdx,iClientCount);
    iClientIdx++;
    return 0;
}
int     PingPangServerHandler::OnClientDataRecv(TcpSocket &   client,const Buffer & recvBuffer) 
{
    for(int i = 0; i < recvBuffer.iUsed; ++i)
    {
        if(recvBuffer.pBuffer[i] == '\0')
        {
            LOG_INFO("server recv data idx = %d is eos",i);
            client.Send(Buffer("Hello,Client!"));
        }
    }    
    return 0;
}

int     PingPangServerHandler::OnConnectionClosed(TcpSocket &  client)
{
    iClientCount--;
    LOG_INFO("a connection fd = %d closed client total = %d",client.GetFD(),iClientCount);
    return 0;
}


///////////////////////////////////////////////////////////////////////////////////////
int     PingPangClientHandler::OnConnected( TcpSocket &   client)
{
    client.Send(Buffer("Hello,Server!"));
    return 0;
}
//readable
int     PingPangClientHandler::OnDataRecv( TcpSocket &   client,const Buffer & recvBuffer)
{
    for(int i = 0; i < recvBuffer.iUsed; ++i)
    {
        if(recvBuffer.pBuffer[i] == '\0')
        {
            LOG_INFO("client recv data idx = %d is eos",i);
            client.Send(Buffer("Hello,Server!"));
        }
    } 
    return 0;
}
//peer close .
int     PingPangClientHandler::OnDisconnected( TcpSocket &   client)
{
    LOG_INFO("client fd = %d is closed",client.GetFD());
    return 0;
}




