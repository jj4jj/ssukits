#pragma once


class SignalHelper
{
public:
    static   sighandler_t SetSigHandler(int signo,sighandler_t handler)
    {
        return signal(signo,handler);
    }
    static   int IgnoreSignal(int signo)
    {
        return SetSigHandler(signo,SIG_IGN);
    }
    static   int RestoreSignal(int signo)
    {
        return SetSigHandler(signo,SIG_DFL);
    }
    static   int SendProcessSignal(int signo,pid_t pid)
    {
        if( kill(pid,signo) < 0)
        {
            LOG_ERROR("send process = %d signal = %d error no = %d",
                    pid,signo,errno);
            return -1;
        }
        return 0;
    }
    static  void    WaitSignal()
    {
        pause();
    }
    static int AlarmAfterSeconds(int iSeconds)
    {        
        if( alarm(iSeconds) < 0)
        {
            LOG_ERROR("alarm error = %d",errno);
            return -1;
        }
        return 0;
    }
   
    static int SetTimer(int iTimerType,struct itimerval * pTime)
    {
        if(setitimer(iTimerType,pTime,NULL) < 0)
        {
            LOG_ERROR("setitimer error = %d",errno);
            return -1;
        }
        return 0;
    }
    static int  GetTimer(int iTimerType,struct itimerval * pTime)
    {
        if( getitimer(iTimerType,pTime) < 0)
        {
            LOG_ERROR("getitimer error = %d",errno);
            return -1;
        }
        return 0;
    }    
};




