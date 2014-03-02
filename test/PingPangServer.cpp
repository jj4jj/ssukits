#include "PingPangHandler.h"
#include "net/TcpServer.h"

int main()
{
    TcpServer   server;
    //port is 1234
    if(server.Init(SockAddress(1234)))
    {
        return -1;
    }
    TcpServerHandler* pHandler = new PingPangServerHandler();
    server.SetServerHandler(pHandler);
    if(server.Start()!=0)
    {
        return -1;  
    }

    int iProc = 0;
    while(true)
    {
        iProc = server.Loop();
        if(iProc < 0)
        {
            return -1;
        }
        else if(0 == iProc)
        {
            //1ms
            usleep(1000);
        }
    }
    return 0;
}

