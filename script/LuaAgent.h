
#pragma once


#ifdef _USE_SYSTEM_LUA

extern "C" 
{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

#else


#include "lua.hpp"


#endif

//stack model
//   N -> GetTop(),  -1
//   N-1      ,      -2
//   .        ,       .
//   .                .
//   .                .
//   1               -N
// positive index : -n+N+1

//LuaAgent is a luastate helper
class LuaAgent
{
public:
    LuaAgent();
    ~LuaAgent();
    //attach a exist lua state
    void Attach(lua_State* pState);
    //create a lua state
    int Init();
    //
    int LoadFile(const char* pszFileName);
    // load buffer
    int LoadBuffer(const char* pBuffer,size_t zBuffLen);
    int DoFile(const char* pszFileName);
    int LoadString(const char* pszString);
    int DoString(const char* pszString);

    //lua_close
    void    Destroy();
    int     OpenLib(const char * szLibName);
    void    OpenStdLib();
    lua_State*  GetLuaState();
public:
    int     GCStop();
    int     GCRestart();
    void    PushNil();
    void    PushNumber(double number);
    void    PushInteger(ptrdiff_t n);
    void    PushLString(const char* pBuffer,size_t zBufflen);
    void    PushString(const char* pszString);
    void    PushBoolean(bool b);
    void    PushCClosure(lua_CFunction f,int n);
    void    PushLightUserData(void * pud);
    const char* GetTypeName(int type);
    int     GetType(int idx);
    bool        IsNil(int idx);
    double      ToNumber(int idx);
    ptrdiff_t   ToInteger(int idx);
    bool         ToBoolean(int idx);
    const char* ToLString(int idx,size_t * len = NULL);
    lua_CFunction ToCFunction(int idx);

    int     GetTop();
    void    SetTop(int idx);
    //remove a stack elment
    void    Remove(int idx);
    //move top element to a pos-1 (top dir) (lenth===)
    void    Insert(int idx);
    //move top elemtn to a pos (lenth--)
    void    Replace(int idx);
    //check stack size
    int     CheckStack(int sz);
    //Pop n elements from stack
    void    Pop(int n);

    void    GetTable(int idx);
    void    GetField(int idx,const char* pszKey);
    void    SetTable(int idx);
    void    SetField(int idx,const char* pszKey);
    void    GetGlobal(const char* pszKey);
    void    SetGlobal(const char* pszKey);
    
    //protected call with a err handler
    int     PCall(int nArgs,int nResults,int errFuncIdx);
    //consistent protected call 
    //int     CPCall(int nArgs,int nResults,int errFunc);
    //direct call
    //int     Call();        

private:
    lua_State* luaState;
};

#define EXPORT_FUNCTION(ls,func)    \
	do{\
        LOG_INFO("push lua function  %s = %p",#func,func);\
	    lua_register(ls,#func,func);\
	}while(0)

#define EXPORT_MACRO(ls,mcr)    \
	do{\
        LOG_INFO("push lua macro  %s = %d",#mcr,mcr);\
		lua_pushinteger((ls),(mcr));\
		lua_setglobal((ls),#mcr);\
	}while(0)
#define EXPORT_VARIABLE(ls,var) \
    do{\
        LOG_INFO("push lua variable  %s = %p",#var,var);\
		lua_pushlightuserdata((ls),(var));\
		lua_setglobal((ls),#var);\
    }while(0)

