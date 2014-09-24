#pragma once

#include "UdpSocketHandler.h"
#include "Epoll.h"


class UdpDriver
{
public:
    int     Init(int maxfds);
    int     AddSocket(int fd);
    int     DelSocket(int fd);
    int     Loop(int iProcNumOneRound,int iTimeOutMS = 10);
    void    SetHandler(UdpSocketHandlerSharedPtr pHdlr); 
public:
    ~UdpDriver();
private:
    Epoll   epoll;
    epoll_event* pEvents;
    int     iProcNum;
    int     iNfds;
    int     iMaxFDs;
    UdpSocketHandlerSharedPtr   ptrHandler;
};





