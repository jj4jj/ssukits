
#pragma once

#ifdef _USE_SYSTEM_LUA

extern "C" {
    #include "lua.h"
    #include "lualib.h"
    #include "lauxlib.h"
}

#else
    #include "dep/lua-5.2.3/include/lua.hpp"
#endif

#include "dep/LuaBridge/LuaBridge.h"















