#pragma once

#include "UdpSocket.h"


class UdpSocketHandler
{
public:
    virtual int OnDataRecv(UdpSocket& udpSock,const Buffer & recvBuffer) = 0;
public:
    virtual int OnReadable(int fd);
    UdpSocketHandler();
    ~UdpSocketHandler();
private:
    enum
    {
        MAX_DEF_RECV_BUFFER_LEN = 4096,
    };
    Buffer      _recvBuffer;
};

typedef shared_ptr<UdpSocketHandler> UdpSocketHandlerSharedPtr;








