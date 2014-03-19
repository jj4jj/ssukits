


#include "SignalHelper.h"


sighandler_t SignalHelper::SetSigHandler(int signo,sighandler_t handler)
{
    return signal(signo,handler);
}
int SignalHelper::IgnoreSignal(int signo)
{
    return SetSigHandler(signo,SIG_IGN);
}
int SignalHelper::RestoreSignal(int signo)
{
    return SetSigHandler(signo,SIG_DFL);
}
int SignalHelper::SendProcessSignal(int signo,pid_t pid)
{
    if( kill(pid,signo) < 0)
    {
        LOG_ERROR("send process = %d signal = %d error no = %d",
                pid,signo,errno);
        return -1;
    }
    return 0;
}
void    SignalHelper::WaitSignal()
{
    pause();
}
int SignalHelper::AlarmAfterSeconds(int iSeconds)
{        
    if( alarm(iSeconds) < 0)
    {
        LOG_ERROR("alarm error = %d",errno);
        return -1;
    }
    return 0;
}

int SignalHelper::SetTimer(int iTimerType,struct itimerval * pTime)
{
    if(setitimer(iTimerType,pTime,NULL) < 0)
    {
        LOG_ERROR("setitimer error = %d",errno);
        return -1;
    }
    return 0;
}
int  SignalHelper::GetTimer(int iTimerType,struct itimerval * pTime)
{
    if( getitimer(iTimerType,pTime) < 0)
    {
        LOG_ERROR("getitimer error = %d",errno);
        return -1;
    }
    return 0;
}    


