#pragma once

#include "Socket.h"

class UdpSocket : public Socket
{
public:
    //init a socket with udp option.
    int     Init();
};







