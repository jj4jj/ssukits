#include "net/UdpSocketHandler.h"
#include "net/UdpDriver.h"
#include "PingPangHandler.h"

int main()
{

    UdpDriver  drv;
    drv.Init(1);
    UdpSocketHandlerSharedPtr pHdlr(new PingPangUdpHandler());
    drv.SetHandler(pHdlr);
    UdpSocket svr;
    if(svr.Init())
    {
        return -1;
    }
    if(svr.Bind(SockAddress(1234,"127.0.0.1")))
    {
        return -1;
    }
    drv.AddSocket(svr.GetFD());
    while(1)
    {
        if(drv.Loop(1,10) < 0)
        {
            return -1;
        }
    }
    return 0;
}









