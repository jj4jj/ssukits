#pragma once

#include "Socket.h"
//todo
class UnixDomainSocket : public Socket
{
public:
    int     Init(int iSockType = SOCK_STREAM);
};




