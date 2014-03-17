#include "Daemon.h"
#include "base/Log.h"


//enbale this process become a daemon process.
int    Daemon::Create(const char* pszWorkDir,bool bCloseStdFD,mode_t mask)
{
//daemon(3)
//Feature Test Macro Requirements for glibc (see feature_test_macros(7)):    
#if _BSD_SOURCE || (_XOPEN_SOURCE && _XOPEN_SOURCE < 500)
    //int daemon ( int __nochdir, int __noclose) ;
    return daemon(strcmp(pszWorkDir,"/")==0?0:1,bCloseStdFD?0:1);
#else
    //1.fork a child process . exit parent process
    //      enable the process into back end running.
    pid_t pid = fork ();
    if(pid > 0)
    {
        //parent process exit.
        LOG_INFO("parent process = %u exit its child = %d",getpid(),pid);
        exit(0);
    }
    
    //2.escape from terminal , login session and process group
    //      process group id is the leader process id.
    //      the cntril terminal of a process group is the 
    //      login terminal of processs group leader .
    //      if it's leader , it's will control the
    //      process group shared terminal .

    if(setsid() < 0)
    {
        LOG_ERROR("daemonlizing setsid error !");
        return -1;
    }

    //3.forbiden opening terminal
    //      this process is a session leader. it can reopen a terminal
    //      fork will disable its leadership.
    pid = fork();
    if(pid > 0)
    {
        LOG_INFO("parent process = %u exit its child = %d",getpid(),pid);            
        exit(0);
    }
    

    //4.close std fd
    //  redirect them to dev/null device
    if(bCloseStdFD)
    {
        close(0);
        open ("/dev/null",O_RDWR,0);
        dup2(0,1);
        dup2(0,1);
    }
    
    
    //5.change current work dir
    //      it's optional.
    if(NULL == pszWorkDir)
    {
        if(chdir(pszWorkDir) < 0)
        {
            LOG_ERROR("daemonlizing chdir work dir=%s error !",pszWorkDir);
            return -1;
        }
    }
    

    
    //6.set file mask
    //      clear the inheritance file mask
    if(umusk(mask) < 0)
    {
        LOG_ERROR("daemonlizing umusk mask=0x%08x error !",mask);
        return -1;
    }


    //7.handle sigchild signal
    //      it's optional , but when child  process is exit.
    //      if parent process don't wait it , the child process will
    //      become a zombie . just ignore this signal.
    signal(SIGCHLD, SIG_IGN);
#endif
    return 0;
}





