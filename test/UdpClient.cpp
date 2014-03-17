#include "net/UdpSocketHandler.h"
#include "net/UdpDriver.h"
#include "PingPangHandler.h"

int main()
{

    UdpDriver  drv;
    drv.Init(1);

    UdpSocketHandlerSharedPtr pHdlr(new PingPangUdpHandler());
    drv.SetHandler(pHdlr);

    UdpSocket client;
    if(client.Init())
    {
        return -1;
    }
    if(client.ConnectTo(SockAddress(1234,"127.0.0.1")))
    {
        return -1;
    }    
    drv.AddSocket(client.GetFD());
    client.Send(Buffer("Hello,Server!"));
    while(1)
    {
        if(drv.Loop(1) < 0)
        {
            return -1;
        }
    }
    return 0;
}








