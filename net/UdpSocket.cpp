#include "UdpSocket.h"
#include "base/Log.h"

int     UdpSocket::Init()
{
    if(GetFD() >= 0)
    {
        LOG_ERROR("init a updsocket repeat ! fd = %d",GetFD());
        return -1;
    }
    int fd = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
    if(fd < 0)
    {
        LOG_ERROR("call socket error !");
        return -1;
    }
    SetFD(fd); 
    SetNonBlock(true);
    ReuseAddr(true);
    return 0;
}






