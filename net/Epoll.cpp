#include "Epoll.h"
#include "base/Log.h"
#include "base/CommonMacro.h"

Epoll::Epoll()
{
    pEvents  = NULL;
    max_nfds = 100;
}
Epoll::~Epoll()
{
    Destroy();
}


int 	Epoll::Init(int _iMaxFdCount )
{    
	//The size is not the maximum size of the backing store but just a hint  to  the
    //kernel about how to dimension internal structures.
	if(pEvents != NULL || _iMaxFdCount <= 0)
	{
		LOG_FATAL("repeat init or client count num = %d <= 0 is invalid!",_iMaxFdCount);
		return -1;
	}
	int iEventBuffSize = sizeof(struct epoll_event)*_iMaxFdCount;
	max_nfds = _iMaxFdCount;
	pEvents = (epoll_event*)malloc(iEventBuffSize);
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
         SAFE_FREE(pEvents);
		return -1;
	}
    return 0;
}
int Epoll::Ctl(int fd,int flag,int ctl_cmd)
{
	if(fd < 0)
	{
		LOG_ERROR("add socket fd = %d < 0",fd);
		return -1;
	}
	struct epoll_event	ev;
	ev.events = 0;
    ev.events = flag;
	ev.data.fd =  fd;	
	int ret = epoll_ctl(epfd,ctl_cmd,fd,&ev);
	if(ret < 0)
	{
		LOG_ERROR("epoll_ctl ctl  = %d  ev fd = %d ret = %d < 0",ctl_cmd,fd,ret);
		return -1;
	}
     LOG_DEBUG("epoll ctl = %d fd = %d event = %d",ctl_cmd,fd,flag);
	return 0;
}
int 	Epoll::Add(int fd,int flag)
{
    return Ctl(fd,flag,EPOLL_CTL_ADD);
}
int 	Epoll::Mod(int fd,int flag)
{
    return Ctl(fd,flag,EPOLL_CTL_MOD);
}

int 	Epoll::Del(int fd)
{
    return Ctl(fd,0,EPOLL_CTL_DEL);
}
int 	Epoll::Polling(epoll_event* * ppEvents,int iTimeCountMs )
{
    *ppEvents  = pEvents;
	int iNfds = epoll_wait(epfd,pEvents,max_nfds,iTimeCountMs);
	if (iNfds < 0 && errno != EINTR ) 
	{
		LOG_ERROR("epoll_wait return ret = %d < 0 errno = %d str = %s",
            iNfds,errno,strerror(errno));
		return iNfds;
	}
	if(iNfds > 0)
	{
		return iNfds;
	}
	//LOG_DEBUG("epoll wait return events num = %d",iNfds);    
	return 0;
}
void	Epoll::Destroy()
{
	if(epfd >= 0 )
	{
		close(epfd);
        	epfd = -1;
	}
    SAFE_FREE(pEvents);
}



