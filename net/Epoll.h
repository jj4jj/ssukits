#pragma once

#include "base/stdinc.h"

class Epoll
{
public:     
    Epoll();
    ~Epoll();
public:
    int     Init(int iMaxfd = DEFAULT_MAX_NFDS);
    void    Destroy();
    int     Add(int fd,int flag);
    int     Mod(int fd,int flag);
    int     Del(int fd);
    //<0  : error
    //>=0 . return *ppEvents
    int     Polling(epoll_event * * ppEvents,int iTimeOutMs = 10); 
private:
    int Ctl(int fd,int flag,int ctl_cmd);
    enum{
        DEFAULT_MAX_NFDS = 1024,
    };
    int     epfd;
    int     max_nfds;
    epoll_event *   pEvents;    
};







