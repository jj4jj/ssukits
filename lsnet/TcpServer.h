/*
 * =====================================================================================
 *
 *       Filename:  TcpServer.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2014年05月22日 05时00分07秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  sean
 *   Organization:  
 *   Last Changed: 05/22/2014 12时20分44秒
 *
 * =====================================================================================
 */


#pragma once

#include "TcpSocket.h"

class TcpConnHandler
{
public:
	TcpConnHandler():pHostSocket(NULL),pHostPoller(NULL){}
	TcpConnHandler(TcpSocket * pSocket):pHostSocket(pSocket){}
	void 	SetHostSocket(TcpSocket* pSocket){pHostSocket = pSocket;}
	void	SetPoller(TcpConnPoller* pPoller){pHostPoller = pPoller;};
	//for server
	int	OnAcceptable();
	//for client
	int	OnConnected();	
	//common handler
	int	OnRecvable();
private:
	TcpSocket*	pHostSocket;
	TcpConnPoller*  pHostPoller;
private:
	TcpConnHandler(const TcpConnHandler &);
	TcpConnHandler & operator = (TcpConnHandler &);
};




struct TcpConnPollerEvent
{
	uint8_t  bEventType;
	enum EventType{
		ET_ACCEPT = 1,
		ET_READ = 2,
		ET_WRITE = 3,	
		ET_CONNECT = 4,
	};
	uint8_t bPollerTag;
	union RawEventData
	{
		epoll_event	ep_ev;
	} raw_event;   	      /*  User data variable */
	
}
enum TcpConnPollerType
{
	TCP_CONN_POLLER_TYPE_BASE = 0;
	TCP_CONN_POLLER_TYPE_EPOLL  = 1;
	TCP_CONN_POLLER_TYPE_SELECT = 2;
	TCP_CONN_POLLER_TYPE_POLL = 3;
	TCP_CONN_POLLER_TYPE_KQUEUE = 4;
	TCP_CONN_POLLER_TYPE_IOCP = 5;
};

//prototype 
class TcpConnPoller 
{
public:
	virtual ~TcpConnPoller();
	// return 0 if ok else err; 
	virtual int 	Init();
	virtual int 	Add(const TcpSocket& sock);
	virtual int 	Del(const TcpSocket& sock);
	virtual int 	Polling(std::vector<TcpConnPollerEvent> & events,int iTimeCountMs);
	virtual void	Destroy();
	virtual TcpSocket* GetTcpSocketFromEvent(TcpConnPollerEvent& event);
};

class TcpConnEpoll : public TcpConnPoller
{
public:
	TcpConnEpoll():epfd(-1),pEvents(NULL){iMaxClientCount = 0;}
	~TcpConnEpoll(){Destroy();}
	// return 0 if ok else err; 
	int 	Init(int iMaxFdCount = 0)
	{
		//The size is not the maximum size of the backing store but just a hint  to  the
       		//kernel about how to dimension internal structures.
       		if(0 == iMaxFdCount){iMaxFdCount = 1024;}
       		if(pEvents != NULL)
		{

			LOG_FATAL("repeat init !");
			return -1;
		}
		int iEventBuffSize = sizeof(struct epoll_event)*iMaxFdCount;
		iMaxClientCount = iMaxFdCount;
		pEvents = malloc(iEventBuffSize);
		if(!pEvents)
		{
			LOG_FATAL("alloc memory error !");
			return -1;
		}
		epfd = epoll_create(iEventBuffSize);
		if(epfd < 0)
		{
			//log err
			LOG_ERROR("epoll_create return epfd = %d < 0",epfd);
			free(pEvents);
			pEvents = NULL;
			return -1;
		}
	}
	int 	Add(const TcpSocket& sock,int iInteredtedEventType)
	{
		int fd = sock.GetFD();
		if(fd < 0)
		{
			LOG_ERROR("add socket fd = %d < 0",fd);
			return -1;
		}
		struct epoll_event	ev;
		ev.evnts = 0;
		switch(iInteredtedEventType)
		{
		case ET_ACCEPT:
		case ET_READ:
			ev.evnts |= EPOLL_IN ;
			break;
		case ET_CONNECT:
		case ET_WRITE:
			ev.evnts |=  EPOLL_OUT ;
			break;
		}
		ev.data.ptr = &sock;	
		int ret = epoll_ctl(epfd,EPOLL_CTL_ADD,fd,&ev);
		if(ret < 0)
		{
			LOG_ERROR("epoll_ctl add ev fd = %d ret = %d < 0",fd,ret);
			return -1;
		}
		return 0;
	}
	int 	Del(const TcpSocket& sock)
	{
		int fd = sock.GetFD();
		if(fd < 0)
		{
			LOG_ERROR("del socket fd = %d < 0",fd);
			return -1;
		}
		struct epoll_event	ev;
		int ret = epoll_ctl(epfd,EPOLL_CTL_DEL,fd,&ev);
		if(ret < 0)
		{
			LOG_ERROR("epoll_ctl del ev fd = %d ret = %d < 0",fd,ret);
			return -1;
		}
		return 0;
	}
	int 	Polling(std::vector<TcpConnPollerEvent> & events,int iTimeCountMs = 10)
	{
		events.clear();
		int iNfds = epoll_wait(epfd,pEvents,iMaxClientCount,TimeCoutMs);
		if (iNfds > 0) 
		{
			TLOG_ERROR("epoll_wait return ret = %d < 0",iNfds);
			return iNfds;
		}
		int  i = 0 , iProcFD = -1;
		TcpConnPollerEvent	tpe;
		for (i = 0; i < iNfds; i++) 
		{
			tpe.raw_event = pEvents[i].events;
			tpe.bPolleType = TCP_CONN_POLLER_TYPE_EPOLL;
			TcpSocket* pSocket = pEvents[i].data.ptr;
			if(EPOLL_IN & (pEvents[i].events))
			{
				if(pSocket->IsListening())
				{
					tpe.bEventType = TcpConnPollerEvent::ET_ACCEPT;
				}		
				else
				{
					tpe.bEventType = TcpConnPollerEvent::ET_READ;
				}
				events.push_back(tpe);
			}
			if(EPOLL_OUT & (pEvents[i].events))
			{
				if(pSocket->IsConnecting())
				{
					tpe.bEventType = TcpConnPollerEvent::ET_CONNECT;
				}
				else
				{
					tpe.bEventType = TcpConnPollerEvent::ET_WRITE;
				}
				events.push_back(tpe);
			}
		}
		return iNfds;
	}
	void	Destroy()
	{
		if(epfd >= 0 )
		{
			close(epfd);
		}
		if(pEvents != NULL)
		{
			free(pEvents);
		}
		epfd = -1;
		pEvents = NUL;
	}
	TcpSocket * GetTcpSocketFromEvent(TcpConnPollerEvent & event)
	{
		return event.raw_event.data.ptr;
	}
private:
	int epfd;	
	struct epoll_event * pEvents ;
	int	iClientCount;
};
//polling policy will be extened in future.
//now use epoll
template <class TcpConnHandlerType = TcpConnHandler ,
	  class TcpConnPollerType = TcpConnEpoll >
class TcpServer
{
public:
	int 	Init(const SockAddress & localAddr,bool bAsyncSocket = true,
			bool bOpenNagle = false,
			int  iMaxConnNum = 1024 )
	{
		if(listenSocket.Init(lockAddr) != 0)
		{
			return -1;
		}
		listenSocket.SetNonBlock(bAsyncSocket);
		if(poller.Init(iMaxConnNum) != 0)
		{
			return -1;
		}
		return 0;
	}
	TcpSocket&	GetListenSocket(){return listenSocket;}
	int	Start()
	{
		if(listenSocket.Listen() != 0)
		{
			return -1;
		}
		if(poller.Add(&listenSocket) != 0)
		{
			return -1;
		}

		return 0;
	}
	int	Polling(int iProcessNum = 1024)
	{
		static std::vector<TcpConnPollerEvent> & events;
		static int	sProcessNumThisRound = 0;
		if(sProcessNumThisRound >= events.size())
		{
			int iNfds = poller.Polling(events);
			sProcessNumThisRound = 0;
			if(iNfds <= 0)
			{
				return iNfds;
			}	
		}
		for(int i = sProcessNumThisRound;
			i < events.size();
			++i,++sProcessNumThisRound)
		{
			TcpSocket* pSocket = poller.GetTcpSocketFromEvent(events[i]);
			if(NULL == pSocket)
			{
				LOG_FATAL("get socket error !");
				continue;
			}
			switch(events[i].bEventType)
			{
				case ET_ACCEPT:
				tcpConnHandler.OnAcceptable();
				break;
				case ET_READ:
				tcpConnHandler.OnRecvable();
				break;
				case ET_WRITE:
				tcpConnHandler.OnSendable();
				break;
				case ET_CONNECT:
				tcpConnHandler.OnConnected();
				break;
			}
		}	
	}
private:
	TcpSocket		listenSocket;	
	TcpConnPollerType	poller;
	TcpConnHandlerType	tcpConnHandler;
};
