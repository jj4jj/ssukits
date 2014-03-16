
#pragma once

template<class PollType ,
         class PollEvent ,
         class HandlerSharedPtr>
class Reactor
{
public:
    int     Init(int maxfds);
    int     AddSocket(int fd,int iEvt = POLL_EVT_IN);
    int     DelSocket(int fd);
    int     Loop(int iProcNumOneRound = 100,int iTimeOutMS = 10);
    void    SetHandler(HandlerSharedPtr pHdlr); 
public:
    ~Reactor();
private:
    PollType   poll;
    PollEvent* pEvents;
    HandlerSharedPtr   ptrHandler;
    int     iProcNum;
    int     iNfds;
    int     iMaxFDs;
    enum PollEventType
    {
        POLL_EVT_IN = 1,
        POLL_EVT_OUT = 2,            
    };       
};


//////////////////////////////////////////////////////////

int     Reactor::Init(int maxfds)
{
    if(poll.Init(maxfds))
    {
        return -1;
    }
    iMaxFDs = maxfds;
    pEvents = NULL;
    iNfds = 0;
    iProcNum = 0;        
    return 0;
}
int     Reactor::AddSocket(int fd,int iEvt)
{
    return    poll.Add(fd,iEvt);
}
int     Reactor::DelSocket(int fd)
{
    return poll.Del(fd);
}

Reactor::~Reactor()
{
    poll.Destroy();
    pEvents = NULL;
    iNfds = 0;
    iProcNum = 0;        
}

int     Reactor::Loop(int iProcNumOneRound,int iTimeOutMS )
{
    if(iProcNum >= iNfds)
    {
        iProcNum = 0;
        iNfds = poll.Polling(&pEvents,iTimeOutMS);
    }
    if(iNfds < 0)
    {
        return -1;
    }
    for(int i = 0; i < iProcNumOneRound &&
                    iProcNum < iNfds ; ++i,++iProcNum)
    {
        if(pEvents[i].events & POLL_EVT_IN)
        {
            ptrHandler->OnReadable(pEvents[i].fd);
        }
        if(pEvents[i].events & POLL_EVT_OUT)
        {
            ptrHandler->OnWritable(pEvents[i].fd);
        }
    }
    return 0;
}
void Reactor::SetHandler(UdpSocketHandlerSharedPtr pHdlr)
{
    ptrHandler = pHdlr;
} 











