#pragma once


class SignalHelper
{
public:
    static   sighandler_t SetSigHandler(int signo,sighandler_t handler);
    static   int IgnoreSignal(int signo);
    static   int RestoreSignal(int signo);
    static   int SendProcessSignal(int signo,pid_t pid);
    static  void    WaitSignal();
    static  int  AlarmAfterSeconds(int iSeconds);   
    static  int  SetTimer(int iTimerType,struct itimerval * pTime);
    static  int  GetTimer(int iTimerType,struct itimerval * pTime);
};




