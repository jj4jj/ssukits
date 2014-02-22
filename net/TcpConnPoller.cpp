#include "TcpConnPoller.h"


//poller
// return 0 if ok else err; 
int 	TcpConnEpoll::Init(int _iMaxFdCount = 0)
{
	//The size is not the maximum size of the backing store but just a hint  to  the
		//kernel about how to dimension internal structures.
	if(0 == _iMaxFdCount){_iMaxFdCount = 1024;}
	if(pEvents != NULL)
	{

		LOG_FATAL("repeat init !");
		return -1;
	}
	int iEventBuffSize = sizeof(struct epoll_event)*_iMaxFdCount;
	iMaxFDCount = _iMaxFdCount;
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
int 	TcpConnEpoll::Add(const TcpSocket& sock,int iInteredtedEventType)
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
		ev.evnts |= EPOLLIN ;
		break;
	case ET_CONNECT:
	case ET_WRITE:
		ev.evnts |=  EPOLLOUT ;
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
int 	TcpConnEpoll::Del(const TcpSocket& sock)
{
	int fd = sock.GetFD();
	if(fd < 0)
	{
		LOG_ERROR("del socket fd = %d < 0",fd);
		return -1;
	}
	struct epoll_event	ev;
	ev.events = (EPOLLIN | EPOLLOUT);
	int ret = epoll_ctl(epfd,EPOLL_CTL_DEL,fd,&ev);
	if(ret < 0)
	{
		LOG_ERROR("epoll_ctl del ev fd = %d ret = %d < 0",fd,ret);
		return -1;
	}
	return 0;
}
int 	TcpConnEpoll::Polling(std::vector<TcpConnPollerEvent> & events,int iTimeCountMs = 10)
{
	events.clear();
	int iNfds = epoll_wait(epfd,pEvents,iMaxFDCount,iTimeCountMs);
	if (iNfds > 0) 
	{
		LOG_ERROR("epoll_wait return ret = %d < 0",iNfds);
		return iNfds;
	}
	int  i = 0 , iProcFD = -1;
	TcpConnPollerEvent	tpe;
	for (i = 0; i < iNfds; i++) 
	{
		tpe.raw_event = pEvents[i].events;
		tpe.bPolleType = TCP_CONN_POLLER_TYPE_EPOLL;
		TcpSocket* pSocket = GetTcpSocketFromEvent(tpe);
		if(EPOLLIN & (pEvents[i].events))
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
		if(EPOLLOUT & (pEvents[i].events))
		{
			if(pSocket->IsConnecting())
			{
				//query status ?
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
void	TcpConnEpoll::Destroy()
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
	pEvents = NULL;
}
TcpSocket * TcpConnEpoll::GetTcpSocketFromEvent(TcpConnPollerEvent & event)
{
	return event.raw_event.data.ptr;
}



