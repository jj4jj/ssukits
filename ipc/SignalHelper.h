#pragma once

#include "base/stdinc.h"
#define restrict    
class SignalHelper
{
public:
    static   sighandler_t SetSigHandler(int signo,sighandler_t handler);
    static   sighandler_t IgnoreSignal(int signo);
    static   sighandler_t RestoreSignal(int signo);
    static   int SendProcessSignal(int signo,pid_t pid);
    static  void    WaitSignal();
    static  void    BlockSignal(int signo);
    static  uint32_t  AlarmInSeconds(int iSeconds);   
    static  int  SetTimer(int iTimerType,struct itimerval * pTime);
    static  int  GetTimer(int iTimerType,struct itimerval * pTime);
    static  int  SendSelfSignal(int signo);
    //reffer to apue
    static  sighandler_t  SignalMayBeUnReliable(int signo,sighandler_t handler);
  
    static  sighandler_t  SignalReliable(int signo,sighandler_t handler);
   
    static  int  SignalAction(int signo,const struct sigaction* restrict act,
                                struct sigaction* restrict oact);
    static  int  ClearSigSet(sigset_t * set);

    static  int  FillSigSet(sigset_t * set);

    static  int  AddSigSet(sigset_t * set, int signo);

    static  int  RemoveSigSet(sigset_t * set, int signo);

    static  bool IsSignalValid(int signo);

    static  bool IsSigInSigSet(sigset_t * set, int signo);

    static int  SetSigSetMask(const sigset_t * restrict set);

    static int  AddSigSetMask(const sigset_t *  restrict set);

    static int  CancelSigSetMask(const sigset_t * restrict set);

    static int  GetSigSetMask(sigset_t *  set);

    static int  GetPendingSigSet(sigset_t *  set);

    static int  SigSusspend(const sigset_t * sigmask);

    static void Abort();

    static const char* GetSignalName(int signo);

    //if first call , return 0
    //else if restore , return the restore param
    //sigsetjmp
    static int  SigalSaveExeContex(jmp_buf  env,int saveMask);
    //siglongjmp
    static void SinalRestoreExeContex(jmp_buf  env,int iCtx);


};




