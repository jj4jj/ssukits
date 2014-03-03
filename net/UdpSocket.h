#pragma once



class UdpSocket : public Socket
{
public:
    int     Init();
    int     Send();
    int     Recv();
    int     Close();
};







