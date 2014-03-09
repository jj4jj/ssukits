#include "UnixDomainSocket.h"
#include "base/Log.h"

int     UnixDomainSocket::Init(int iSockType /*SOCK_STREAM*/)
{
    int fd = socket(AF_UNIX, iSockType, 0);
    if(fd  < 0)
    {
        LOG_FATAL("error create unix domain socket fd = %d errno = %d",fd,errno);
        return -1;
	}
    return 0;
}  



