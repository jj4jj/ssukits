#include "UdpSocketHandler.h"
#include "base/Log.h"

int UdpSocketHandler::OnReadable(int fd)
{
    UdpSocket sock(fd);
    SockAddress addr;
    int ret = sock.RecvFrom(_recvBuffer,addr);
    if(ret == 0)
    {
        return OnDataRecv(sock,_recvBuffer,addr);
    }
    LOG_ERROR("udp socket fd = %d read error from addr = %s!",fd,addr.ToString());
    sock.Close();            
    return -1;
}
UdpSocketHandler::UdpSocketHandler()
{
    _recvBuffer.Create(MAX_DEF_RECV_BUFFER_LEN);
}
UdpSocketHandler::~UdpSocketHandler()
{
    _recvBuffer.Destroy();
}

