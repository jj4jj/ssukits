
#include "base/include.h"


int main()
{
    LOG_TRACE("info test");    
    LOG_DEBUG("debug test");
    LOG_INFO("info test");
    LOG_WARN("warn test");
    LOG_ERROR("error test");
    LOG_FATAL("fatal test");
  
    Log::Instance().Init("test_base.log");
    LOG_TRACE("info test");    
    LOG_DEBUG("debug test");
    LOG_INFO("info test");
    LOG_WARN("warn test");
    LOG_ERROR("error test");
    LOG_FATAL("fatal test");
    
    return 0;
}






