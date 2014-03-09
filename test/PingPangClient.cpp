
#include "base/Log.h"
#include "net/TcpClient.h"
#include "net/BatchTcpConnection.h"
#include "PingPangHandler.h"
int main()
{

    Log::Instance().Init("pingpang_client.log");

#if 1
    //use batch connection 
    BatchTcpConnection  btc;
    if(btc.Init(1))
    {
        return -1;
    }
    if(btc.AddConnection(SockAddress(1234,"127.0.0.1"),TcpClientHandlerSharedPtr( new PingPangClientHandler())))
    {
        return -1;
    }
    while(true)
    {
       int ret = btc.Loop();
       if(ret < 0)
       {
            return -1;
       }
       //no event occur
       else if( 1 == ret)
       {
            usleep(20);
       }
    }
    
#else
    //use client num     
    int iCliNum = 1; 
    Epoll* pPoller = new Epoll();
    if(pPoller->Init(iCliNum))
    {
        return -1;
    }
    std::vector<TcpClient>   clientList;
    SockAddress remote(1234,(char*)"127.0.0.1");
    clientList.resize(iCliNum);
    FDTcpSocketMap      mp;
    for(int i = 0 ; i < iCliNum; ++i)
    {
        TcpClient* pClient = &(clientList[i]);
        if(pClient->Init())
        {
            continue;
        }
        pClient->SetPoller(pPoller);
        if(pClient->ConnectTo(remote))
        {
            continue;
        }
        mp.AddFDTcpSocket(pClient->GetSocket().GetFD(),pClient->GetSocket());
    }
    epoll_event * pEvents = NULL;
    int nfds = 0;
    TcpClientHandler * pClientHandler = new PingPangClientHandler();
    pClientHandler->SetPoller(pPoller);
    pClientHandler->SetFD2TcpSocketMap(&mp);    
    while(true)
    {
        nfds = pPoller->Polling(&pEvents,iCliNum);
        if(nfds < 0)
        {
            LOG_FATAL("client polling error !");
            return -1;
        }
        if(0 == nfds)
        {
            usleep(10);
        }
        for(int i = 0;i < nfds; ++i)
        {
            int fd = pEvents[i].data.fd;
            if(pEvents[i].events & EPOLLIN )
            {
                pClientHandler->OnReadable(fd);
            }
            if(pEvents[i].events & EPOLLOUT )
            {
                pClientHandler->OnWritable(fd);
            }
        }        
    }
#endif    
    return 0;
}















