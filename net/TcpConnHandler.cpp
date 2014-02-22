#include "TcpConnHandler.h"
//for server
int	TcpConnHandler::OnAcceptable()
{
	LOG_TRACE("new client comming ..");
	return 0;
}
//for client
int	TcpConnHandler::OnConnected()
{
	LOG_TRACE("client connect succes ..");
	return 0;
}
//common handler
int	TcpConnHandler::OnRecvable()
{
	LOG_TRACE("peer has data comming ..");
	return 0;
}

////////////////////////////////////////////////////////////

//ping-pang handler
class PingPangHandler : TcpConnHandler
{
	//for server
	//when new client is comming . 
	//recv its data and echo it.
	int	OnAcceptable()
	{
		TcpSocket & newClient = pHostSocket->Accept();
		pHostPoller->Add(newClient,ET_READ);
		return 0;
	}
	//for client
	//send hello,world
	int	OnConnected()
	{	
		if(NULL != pHostPoller)
		{
			pHostPoller->Del(pHostSocket);
		}
		if(pHostSocket->Send(Buffer("hello,world!")) < 0)
		{
			pHostSocket->Close();
			return -1;
		}
		//
		if(NULL != pHostPoller)
		{
			pHostPoller->Add(pHostSocket,ET_READ);
		}		
		return 0;		
	}
	//common handler
	//recv msg , then send over.
	int	OnRecvable()
	{
		
		if(pHost->Recv(Buffer(szBuffer+iReceived,sizeof(szBuffer)-iReceived)) < 0)
		{
			if(NULL != pHostPoller)
			{
				pHostPoller->Del(pHostSocket);			
			}
			pHostSocket->Close();
			return -1;
		}
		else
		{
			if(szBuffer[iReceived-1] == '\0')
			{
				pHostSocket->Send(szBuffer,iReceived);
			}
		}	
		return 0;
	}
}

