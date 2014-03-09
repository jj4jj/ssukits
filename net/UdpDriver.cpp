#include "UdpDriver.h"


int     UdpDriver::Init(int maxfds)
{
    if(epoll.Init(maxfds))
    {
        return -1;
    }
    iMaxFDs = maxfds;
    pEvents = NULL;
    iNfds = 0;
    iProcNum = 0;        
    return 0;
}
int     UdpDriver::Loop(int iProcNumOneRound,int iTimeOutMS )
{
    if(iProcNum >= iNfds)
    {
        iProcNum = 0;
        iNfds = epoll.Polling(&pEvents,iTimeOutMS);
    }
    if(iNfds < 0)
    {
        return -1;
    }
    for(int i = 0; i < iProcNumOneRound &&
                    iProcNum < iNfds ; ++i,++iProcNum)
    {
        if(pEvents[i].events & EPOLLIN)
        {
            pHandler->OnReadable(pEvents[i].data.fd);
        }
    }
    return 0;
}
void UdpDriver::SetHandler(UdpSocketHandlerSharedPtr pHdlr)
{
    ptrHandler = pHdlr;
}    
