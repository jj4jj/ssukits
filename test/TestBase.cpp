
#include "base/include.h"
#include "utility/Daemon.h"

int main()
{
    LOG_TRACE("info test");    
    LOG_DEBUG("debug test");
    LOG_INFO("info test");
    LOG_WARN("warn test");
    LOG_ERROR("error test");
    LOG_FATAL("fatal test");

    if(0 == Daemon::Instance().Create())
    {
        LOG_ERROR("daemonlize the process fail !");
        return -1;
    }
  
    Log::Instance().Init("test_base.log");
    LOG_TRACE("info test");    
    LOG_DEBUG("debug test");
    LOG_INFO("info test");
    LOG_WARN("warn test");
    LOG_ERROR("error test");
    LOG_FATAL("fatal test");
    
    while(true)
    {                
        sleep(1);
    }
    
    
    return 0;
}






