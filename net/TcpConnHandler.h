
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
	virtual int	OnDataSent(const Buffer & sendBuffer);
public:	    
	// when new client is comming , accept it .
	virtual int	OnAcceptable();
	//for client , the connection is ok .
	virtual int	OnConnected();
	//by self or peer
	virtual int	OnClosed(bool bBySelf);
	//common handler
	virtual int	OnReadable();
public:
	void     SetHostSocket(TcpSocket* pSocket){pHostSocket = pSocket;}
	void	     SetHostPoller(TcpConnPoller* pPoller){pHostPoller = pPoller;}	
    	TcpSocket*	GetHostSocket(){return pHostSocket;}
	TcpConnPoller*  GetHostPoller(){return pHostPoller;}
public:
	//for sever call.
    void	construct();
    void destruct();
	TcpConnHandler(){construct();}
	TcpConnHandler(TcpSocket * pSocket){construct();pHostSocket = pSocket;}
	virtual ~TcpConnHandler(){destruct();}
private:
	TcpSocket*	    pHostSocket;
	TcpConnPoller* 	pHostPoller;
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
	int	OnNewConnection(TcpSocket& newSock);
	//for client
	int	OnConnxSuccess();	
	//by self or peer
	int	OnConnxClosed(bool bBySelf );
	//common handler
	int	OnDataRecv(const Buffer & recvBuffer);

    PingPangHandler():iReceived(0){}
private:
	char szBuffer[128];
	int  iReceived;
};

