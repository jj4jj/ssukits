
#pragma once

#include "TcpClient.h"

class BatchTcpConnection
{
public:
    int     Init(int _iMaxConnxNum = 100);
    int     SetDefHandler(TcpClientHandlerSharedPtr pHandler);
    int     AddConnection(const SockAddress& remote,TcpClientHandlerSharedPtr pHandler );
    int     Loop(int iProcNum = 100);
    int     Shutdown();
private:    
    Epoll               epoll;  
    epoll_event *       pEvents;  
    int                 iNfds;
    int                 iLastProcNum;
    int                 iMaxConnxNum;
    std::vector<TcpClient>   clientList;   
typedef std::map<int,shared_ptr<TcpClientHandler> > ClientHandlerMap;
typedef ClientHandlerMap::iterator      ClientHandlerMapItr;
    ClientHandlerMap                    mpClientHandler;    
    shared_ptr<TcpClientHandler>        ptrClientDefHandler;    
};




