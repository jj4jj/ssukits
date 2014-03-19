
#pragma once

#ifdef _USE_SYSTEM_LUA

extern "C" {
    #include "lua.h"
    #include "lualib.h"
    #include "lauxlib.h"
}

#else
    #include "lua.hpp"
#endif

#include "LuaBridge.h"















