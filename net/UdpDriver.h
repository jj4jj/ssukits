#pragma once

#include "UdpSocketHandler.h"


class UdpDriver
{
public:
    int     Init(int maxfds);
    int     Loop(int iProcNumOneRound,int iTimeOutMS = 10);
    void SetHandler(UdpSocketHandlerSharedPtr pHdlr); 
private:
    Epoll   epoll;
    epoll_event* pEvents;
    int     iProcNum;
    int     iNfds;
    int     iMaxFDs;
    UdpSocketHandlerSharedPtr   ptrHandler;
};





