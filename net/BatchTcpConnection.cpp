

#include "BatchTcpConnection.h"
#include "base/Log.h"


int     BatchTcpConnection::Init(int _iMaxConnxNum)
{
    if(epoll.Init(_iMaxConnxNum))
    {
        return -1;
    }
    clientList.reserve(_iMaxConnxNum);
    iMaxConnxNum = _iMaxConnxNum;
    return 0;
}       
int     BatchTcpConnection::SetDefHandler(TcpClientHandler* pHandler)
{
    if(NULL == pHandler)
    {
        LOG_FATAL("set default handler is null ");
        return -1;
    }
    ptrClientDefHandler.reset(pHandler);
    return 0;
}
int     BatchTcpConnection::AddConnection(const SockAddress& remote,TcpClientHandler* pHandler)
{        
    TcpClient   tcl;
    if(tcl.Init())
    {
        return -1;;
    }
    tcl.SetPoller(&epoll);
    if(tcl.ConnectTo(remote))
    {
        tcl.Close();
        return -1;
    }
    mp.AddFDTcpSocket(tcl.GetSocket().GetFD(),tcl.GetSocket());
    if(NULL != pHandler)
    {
        pHandler->SetPoller(&epoll);
        pHandler->SetFD2TcpSocketMap(&mp);    
        shared_ptr<TcpClientHandler> p;
        p.reset(pHandler);
        mpClientHandler.insert(std::make_pair(tcl.GetSocket().GetFD(),p));
    }
    clientList.push_back(TcpClient());        
    return 0;
}
int     BatchTcpConnection::Loop(int iProcNum)
{
    if(iLastProcNum >= iNfds)
    {
        iLastProcNum = 0;
        iNfds = epoll.Polling(&pEvents,iMaxConnxNum);
        if(iNfds < 0)
        {
            LOG_FATAL("client polling error !");
            return -1;
        }            
    }
    if(0 == iNfds)
    {
        //no events
        return 1;
    }
    for(int i = 0;i < iProcNum &&
                  iLastProcNum < iNfds;
                  ++i,++iLastProcNum )
    {
        int fd = pEvents[i].data.fd;
        ClientHandlerMapItr it = mpClientHandler.find(fd);
        TcpClientHandler* pHandler = NULL;
        if(it != mpClientHandler.end())
        {
            pHandler = it->second.get();
        }
        else
        {
            pHandler = ptrClientDefHandler.get();
        }
        if(pEvents[i].events & EPOLLIN )
        {
            pHandler->OnReadable(fd);
        }
        if(pEvents[i].events & EPOLLOUT )
        {
            pHandler->OnWritable(fd);
        }
    }
    return 0;
}
int     BatchTcpConnection::Shutdown()
{
    for(int i = 0 ; i < (int)clientList.size() ; ++i)
    {
        clientList[i].Close();
    }
    clientList.clear(); 
    mpClientHandler.clear();
    epoll.Destroy();
    return 0;
}





