#include "UdpSocketHandler.h"
#include "base/Log.h"

int UdpSocketHandler::OnReadable(int fd)
{
    UdpSocket sock(fd);
    int ret = sock.Recv(_recvBuffer);
    if(ret == 0)
    {
        return OnDataRecv(sock,_recvBuffer);
    }
    LOG_ERROR("udp socket fd = %d read error !",fd);
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

