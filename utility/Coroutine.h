#pragma once



class Coroutine
{
public:
    int     Yield();
    int     Exit();
public:
    int     Start();
    int     Resume();
};



class CoroutineMgr
{
public:
    int         Create();
    int         Destroy(int iNextCoID);     

private:
    int         iNextCoID;
};










