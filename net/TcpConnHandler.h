
#pragma once
#include "TcpSocket.h"

// a trace handler , nothing to do.
class TcpConnHandler
{
public:
	//call back when event be triggered
	//for server offen set handler ro sth.
	virtual int	OnNewConnection(TcpSocket& newSock);
	//for client
	virtual int	OnConnxSuccess();	
	//by self or peer
	virtual int	OnConnxClosed(bool bBySelf );
	//common handler
	virtual int	OnDataRecv(const Buffer & recvBuffer);
	virtual int	OnDataSent(const Buffer & recvBuffer);
public:	
	// when new client is comming , accept it .
	virtual int	OnAcceptable()
	{
		//
		struct sockaddr addr;
		socklen_t	addrlen = sizeof(addr);
		int clifd = accept(pHostSocket->GetFD(),&addr,&addrlen);
		if( clifd < 0)
		{
			LOG_ERROR("accept error no = %d!",errno);
			return -1;
		}		
		TcpSocket * pClientSocket = new TcpSocket();
		if(NULL == pClientSocket)
		{
			LOG_FATAL("new TcpSocket obj  error !");
			return -1;
		}
		//-map it . for managing
		//use a handle
		//handle -> socket(fd)
		PushNewSocket(pClientSocket);	
		
		pClientSocket->AttachFD(clifd);
		pClientSocket->SetLocalAddress(pHostSocket->GetLocalAddress());
		pClientSocket->SetRemoteAddress(SocketAdress(addr));
		pClientSocket->SetConnFlag(TcpSocket::CONN_FLAG_ESTABLISHED);

		return  OnNewConnection(*pClientSocket))
	}
	//for client , the connection is ok .
	virtual int	OnConnected()
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
	virtual int	OnClosed(bool bBySelf)
	{
		if(NULL != pHostPoller)
		{
			//if it's in poller
			pHostPoller->Del(*pHostSocket);
		}
		return pHostSocket->OnConnxClosed(bBySelf);		
	}
	//common handler
	virtual int	OnReadable()
	{		
		int iRecv = pHostSocket->Recv();
		if(iRecv > 0)
		{
			//-
		}
		return 0;
	}
public:
	void 	SetHostSocket(TcpSocket* pSocket){pHostSocket = pSocket;}
	void	SetHostPoller(TcpConnPoller* pPoller){pHostPoller = pPoller; }	
	
public:
	//for sever call.
	TcpConnHandler():pHostSocket(NULL),pHostPoller(NULL)
	{
		connxMapHandle2Socket.clear();
	}
	TcpConnHandler(TcpSocket * pSocket):pHostSocket(pSocket){}
	virtual ~TcpConnHandler(){pHostSocket=NULL,pHostPoller=NULL;};
private:
	TcpSocket*	pHostSocket;
	TcpConnPoller* 	pHostPoller;
typedef std::map<uint32_t,TcpSocket*>	ConnxMap;
typedef ConnxMap::iterator			    ConnxMapIterator;	
	ConnxMap							connxMapHandle2Socket;
private:
	TcpConnHandler(const TcpConnHandler &);
	TcpConnHandler & operator = (TcpConnHandler &);
};


//ping-pang handler
class PingPangHandler : TcpConnHandler
{
public:
	//for server
	//when new client is comming . 
	//recv its data and echo it.
	int	OnAcceptable();
	//for client
	//send hello,world
	int	OnConnected();	
	//common handler
	//recv msg , then send over.
	int	OnRecvable();
public:
	char szBuffer[128];
	int  iReceived;
}

