#include "SignalHelper.h"
#include "base/Log.h"

#ifndef SIGBAD
#define SIGBAD(signo) ((signo) <= 0 || (signo) >= NSIG)
#endif

#if 1
sighandler_t SignalHelper::SetSigHandler(int signo,sighandler_t handler)
{
    return signal(signo,handler);
}
sighandler_t SignalHelper::IgnoreSignal(int signo)
{
    return SetSigHandler(signo,SIG_IGN);
}
sighandler_t SignalHelper::RestoreSignal(int signo)
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
int  SignalHelper::SendSelfSignal(int signo)
{
    if( raise(signo) < 0)
    {
        LOG_ERROR("send process self signal = %d error no = %d",
                signo,errno);
        return -1;
    }
    return 0;
}
void    SignalHelper::BlockSignal(int signo)
{
    //todo
    
}


void    SignalHelper::WaitSignal()
{
    pause();
}
uint32_t SignalHelper::AlarmInSeconds(int iSeconds)
{        
    return alarm(iSeconds);
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
sighandler_t  SignalHelper::SignalMayBeUnReliable(int signo,sighandler_t handler)
{
    return signal(signo,handler);
}    
sighandler_t  SignalHelper::SignalReliable(int signo,sighandler_t handler)
{
    struct sigaction act,oact;
    act.sa_handler = handler;
    ClearSigSet(&act.sa_mask);
    act.sa_flags = 0;
    if(SIGALRM == signo)
    {
        //will not auto restart
#ifdef  SA_INTERRUPT
    act.sa_flags |= SA_INTERRUPT;
#endif        
    }
    else
    {
        //auto restart
#ifdef  SA_RESTART
    act.sa_flags |= SA_RESTART;
#endif
    }
    if(SignalAction(signo,&act,&oact) < 0)
    {
        LOG_ERROR("sigaction error no = %d",errno);
        return SIG_ERR;
    }
    return (oact.sa_handler);
}
  int  SignalHelper::SignalAction(int signo,const struct sigaction* restrict act,
                            struct sigaction* restrict oact)
{
    return sigaction(signo,act,oact);
}
  int  SignalHelper::ClearSigSet(sigset_t * set)
{
    return      sigemptyset(set);
}
  int  SignalHelper::FillSigSet(sigset_t * set)
{
    return      sigfillset(set);
}

  int  SignalHelper::AddSigSet(sigset_t * set, int signo)
{
    return sigaddset(set,signo);
}
  int  SignalHelper::RemoveSigSet(sigset_t * set, int signo)
{
    return sigdelset(set,signo);
}
  bool SignalHelper::IsSignalValid(int signo)
{
    return (SIGBAD(signo) == 0 );
}
  bool SignalHelper::IsSigInSigSet(sigset_t * set, int signo)
{
    return (sigismember(set,signo) != 0);
}
 int  SignalHelper::SetSigSetMask(const sigset_t * restrict set)
{
    return sigprocmask(SIG_SETMASK,set,NULL);
}
 int  SignalHelper::AddSigSetMask(const sigset_t *  restrict set)
{
    return sigprocmask(SIG_BLOCK,set,NULL);
}
 int  SignalHelper::CancelSigSetMask(const sigset_t * restrict set)
{
    return sigprocmask(SIG_UNBLOCK,set,NULL);
}
 int  SignalHelper::GetSigSetMask(sigset_t *  set)
{
    sigset_t _dummy;
    ClearSigSet(&_dummy);
    return sigprocmask(SIG_BLOCK,&_dummy,set);
}
 int  SignalHelper::GetPendingSigSet(sigset_t *  set)
{
    return sigpending(set);
}
 int  SignalHelper::SigSusspend(const sigset_t * sigmask)
{
    return SigSusspend(sigmask);
}
 void SignalHelper::Abort()
{
    abort();
}
 const char* SignalHelper::GetSignalName(int signo)
{
    return strsignal(signo);
}
//if first call , return 0
//else if restore , return the restore param
 int SignalHelper::SigalSaveExeContex(jmp_buf  env,int saveMask)
{
    return sigsetjmp(env,saveMask);
}
 void SignalHelper::SinalRestoreExeContex(jmp_buf  env,int iCtx)
{
    siglongjmp(env,iCtx);
}

 #endif

