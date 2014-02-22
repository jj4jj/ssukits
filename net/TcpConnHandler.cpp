#include "TcpConnHandler.h"



#if 1
void	TcpConnHandler::construct()
{
    pHostSocket = NULL;
    pHostPoller = NULL;
}
void TcpConnHandler::destruct()
{
    pHostSocket = NULL;
    pHostPoller = NULL;
}

#endif




int	TcpConnHandler::OnNewConnection(TcpSocket& newSock)
{
    LOG_TRACE("new client connected .");
    return 0;
}
//for client
int	TcpConnHandler::OnConnxSuccess()
{
    LOG_TRACE("connected ok .");
    return 0;
}
//by self or peer
int	TcpConnHandler::OnConnxClosed(bool bBySelf )
{
    LOG_TRACE("connection closed .");
    return 0;
}
//common handler
int	TcpConnHandler::OnDataRecv(const Buffer & recvBuffer)
{
    LOG_TRACE("recv data .");
    return 0;
}
int	TcpConnHandler::OnDataSent(const Buffer & sendBuffer)
{
    LOG_TRACE("sent data .");
    return 0;
}
int	TcpConnHandler::OnAcceptable()
{
	//
	TcpSocket * pClientSocket = pHostSocket->Accept();
		if( pClientSocket != NULL )
	{            
		return -1;
	}
	return  OnNewConnection(*pClientSocket))
}
//for client , the connection is ok .
int	TcpConnHandler::OnConnected()
{
	int error = 0;  
    socklen_t len = sizeof(int);  
	int ret = getsockopt(pHostSocket->GetFD(),
				SOL_SOCKET, SO_ERROR,
				&error, &len);
    if (ret != 0 ||
		error != 0 )
	{  
			LOG_ERROR("connect error = %d",error);
			//close 
			pHostSocket->Close();
    } 
	else
	{  
		//remove host socket in poller
		if(NULL != pHostPoller)
		{
			pHostPoller->Del(*pHostSocket);
		}				
		return OnConnxSuccess();
	}  
	return 0;
}
//by self or peer
int	TcpConnHandler::OnClosed(bool bBySelf)
{
	if(NULL != pHostPoller)
	{
		//if it's in poller
		pHostPoller->Del(*pHostSocket);
	}
	return pHostSocket->OnConnxClosed(bBySelf);		
}
//common handler
int	TcpConnHandler::OnReadable()
{		
	return pHostSocket->Recv();
}



////////////////////////////////////////////////////////////
//ping-pang handler
//server recv a new client
//set its handler 
//add it into poller
int	PingPangHandler::OnNewConnection(TcpSocket& newSock)
{
    //create a handler . but when does it should be free ?
    newSock.SetHandler();
    TcpConnPoller* pPoller = GetHostPoller();
    if(NULL != pPoller)
    {
        pPoller->Add(newSock,TcpConnPollerEvent:::ET_READ);
    }
    return 0;            
}

//for client
int	PingPangHandler::OnConnxSuccess()
{
    //client connect ok , then send hello,world
    return    GetHostSocket()->Send(Buffer("Hello,World!"));
}

//by self or peer
int	PingPangHandler::OnConnxClosed(bool bBySelf )
{
    return 0;
}
//common handler
int	PingPangHandler::OnDataRecv(const Buffer & recvBuffer)
{
    if(iReceived + recvBuffer.iUsed >= sizeof(szBuffer))
    {
        szBuffer[sizeof(szBuffer)-1] = '\0';
        LOG_ERROR("error fmt msg = %s",szBuffer);
        GetHostSocket()->Close();
        return -1;
    }
    memcpy(szBuffer+iReceived,recvBuffer.pBuffer,recvBuffer.iUsed);
    if(recvBuffer.pBuffer[recvBuffer.iUsed-1] == '\0')
    {        
        GetHostSocket()->Send(Buffer(szBuffer,iReceived));
        iReceived = 0;
    }
    return 0;
}




