

#pragma once

#include "TcpSocket.h"

enum TcpConnPollerType
{
	TCP_CONN_POLLER_TYPE_BASE = 0,
	TCP_CONN_POLLER_TYPE_EPOLL  = 1,
	//no support now
	TCP_CONN_POLLER_TYPE_SELECT = 2,
	TCP_CONN_POLLER_TYPE_POLL = 3,
	TCP_CONN_POLLER_TYPE_KQUEUE = 4,
	TCP_CONN_POLLER_TYPE_IOCP = 5,
};

struct TcpConnPollerEvent
{
	uint8_t  bEventType;
	enum EventType{
		ET_ACCEPT = 1,
		ET_CONNECT = 2,
		ET_READ =  3,
		ET_WRITE = 4,
	};
	uint8_t bPollerType;
	union RawEventData
	{
		epoll_event	ep_ev; //for TCP_CONN_POLLER_TYPE_EPOLL
	} raw_event;   	      /*  User data variable */
	
}

//prototype 
//for tcp server , it will polling connection for checking event.
class TcpConnPoller
{
public:
	TcpConnPoller(){}
	virtual ~TcpConnPoller(){}	
	// return 0 if ok else err; 
	virtual int 	Init(){return 0;}
	//add monitor of interested event type
	virtual int 	Add(const TcpSocket& sock,uint8_t bEventType) = 0;
	/del all socket events
	virtual int 	Del(const TcpSocket& sock) = 0;
	//return >= 0 (represent the event count) is ok , else is error.
	virtual int 	Polling(std::vector<TcpConnPollerEvent> & events,int iTimeCountMs) = 0;

	//tcp server will get a the attached data form event.
	virtual TcpSocket* GetTcpSocketFromEvent(const TcpConnPollerEvent& event) = 0;

	//destroy the poller
	void	Destroy(){}
};

class TcpConnEpoll : public TcpConnPoller
{
public:
	TcpConnEpoll():epfd(-1),pEvents(NULL){iMaxClientCount = 0;}
	~TcpConnEpoll(){Destroy();}
	// return 0 if ok else err; 
	// create epoll fd
	int 	Init(int iMaxFdCount = 100);
	int 	Add(const TcpSocket& sock,uint8_t iInteredtedEventType);	
	int 	Del(const TcpSocket& sock);
	int 	Polling(std::vector<TcpConnPollerEvent> & events,int iTimeCountMs = 10);
	void	Destroy();
	TcpSocket * GetTcpSocketFromEvent(TcpConnPollerEvent & event){return event.raw_event.data.ptr;}
private:
	int epfd;	
	struct epoll_event * pEvents ;
	int	iClientCount;
};


