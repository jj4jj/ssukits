
#pragma once


#ifdef _USE_SYSTEM_LUA

extern "C" 
{
    #include "lua.h"
    #include "lualib.h"
    #include "lauxlib.h"
}

#else
    #include "dep/lua-5.2.3/include/lua.hpp"
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
    //
    int LoadBuffer(const char* pBuffer,size_t zBuffLen);

    //lua_close
    void    Destroy();
    int     OpenLib(const char * szLibName);
    lua_State*  GetLuaState();
public:
    void    PushNil();
    void    PushNumber(double number);
    void    PushInteger(ptrdiff_t n);
    void    PushLString(const char* pBuffer,size_t zBufflen);
    void    PushString(const char* pszString);
    void    PushBoolean(bool b);
    void    PushCClosure(lua_CFunction f,int n);
    const char* GetTypeName(int type);
    int     GetType(int idx);

    double      ToNumber(int idx);
    ptrdiff_t   ToInteger(int idx);
    bool         ToBoolean(int idx);
    const char* ToLString(int idx,size_t * len);
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
    int    CheckStack(int sz);
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




