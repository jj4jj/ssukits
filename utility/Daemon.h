#pragma once

#include "base/Singleton.hpp"
#include "base/stdinc.h"

class Daemon : public Singleton<Daemon>
{
public:
    //return 0 is ok, error < 0
    //enbale this process become a daemon process.
    int    Create(const char* pszWorkDir = NULL,bool bCloseStdFD = true,mode_t mask = 0);
};






