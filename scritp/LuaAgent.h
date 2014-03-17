#pragma once
extern "C" 
{
    #include "dep/lua/lua.h"
    #include "dep/lua/lualib.h"
    #include "dep/lua/lauxlib.h"
}

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
    LuaAgent():luaState(NULL){}
    ~LuaAgent()
    {
        Destroy();
    }
    //attach a exist lua state
    void Attach(lua_State* pState)
    {
        Destroy();
        luaState = pState;
    }
    //create a lua state
    int Init()
    {
        luaState = luaL_newstate();
        if(NULL == luaState)
        {
            LOG_FATAL("lua new state error !");
            return -1;
        }
        luaL_openlibs(luaState);

        return 0;
    }
    //
    int LoadFile(const char* pszFileName)
    {
        return luaL_loadfile(luaState,pszFileName)
    }
    //
    int LoadBuffer(const char* pBuffer,size_t zBuffLen)
    {
        return luaL_loadbuffer(luaState,pBuffer,zBuffLen,NULL);
    }

    //lua_close
    void    Destroy()
    {
        if(!luaState)
        {
            lua_close(luaState);
        }
    }
    int     OpenLib(const char * szLibName)
    {
        return luaL_openlib(luaState,szLibName,NULL,0);
    }
    lua_State*  GetLuaState()
    {
        return luaState;
    }
public:
    void    PushNil()
    {
        lua_pushnil(luaState);
    }
    void    PushNumber(double number)
    {
        lua_pushnumber(luaState,number);
    }
    void    PushInteger(ptrdiff_t n)
    {
        lua_pushinteger(luaState,n);
    }
    void    PushLString(const char* pBuffer,size_t zBufflen)
    {
        lua_pushlstring(luaState,pBuffer,zBufflen);
    }
    void    PushString(const char* pszString)
    {
        lua_pushstring(luaState,pszString);
    }
    void    PushBoolean(bool b)
    {
        lua_pushboolean(luaState,b?1:0);
    }
    void    PushCClosure(lua_CFunction f,int n)
    {
        lua_pushcclosure(luaState,f,n);
    }
    const char* GetTypeName(int type)
    {
        return lua_typename(luaState,type)
    }
    int     GetType(int idx)
    {
        return lua_type(luaState,idx);
    }

    double      ToNumber(int idx)
    {
        return lua_tonumber(luaState,idx);
    }
    ptrdiff_t   ToInteger(int idx)
    {
        return lua_tointeger(luaState,idx);
    }
    bool         ToBoolean(int idx)
    {
        return (lua_toboolean(luaState,idx)>0);
    }
    const char* ToLString(int idx,size_t * len)
    {
        return lua_tolstring(luaState,idx,len);
    }
    lua_CFunction ToCFunction(int idx)
    {
        return lua_tocfunction(luaState,idx);
    }

    int     GetTop()
    {
        return lua_gettop(luaState);
    }
    void    SetTop(int idx)
    {
        lua_settop(luaState,idx);
    }
    //remove a stack elment
    void    Remove(int idx)
    {
        lua_remove(luaState,idx);
    }
    //move top element to a pos-1 (top dir) (lenth===)
    void    Insert(int idx)
    {
        lua_insert(luaState,idx);
    }
    //move top elemtn to a pos (lenth--)
    void    Replace(int idx)
    {
        lua_replace(luaState,idx);
    }
    //check stack size
    int    CheckStack(int sz)
    {
        return lua_checkstack(luaState,sz);
    }
    //Pop n elements from stack
    void    Pop(int n)
    {
        lua_pop(luaState,n);
    }

    void    GetTable(int idx)
    {
        lua_gettable(luaState,idx);
    }
    void    GetField(int idx,const char* pszKey)
    {
        lua_getfield(luaState idx,pszKey);
    }
    void    SetTable(int idx)
    {
        lua_settable(luaState,idx);
    }
    void    SetField(int idx,const char* pszKey)
    {
        lua_setfield(luaState idx,pszKey);
    }
    void    GetGlobal(const char* pszKey)
    {
        lua_getglobal(luaState,pszKey);
    }
    void    SetGlobal(const char* pszKey)
    {
        lua_setglobal(luaState,pszKey);
    }
    
    //protected call with a err handler
    int     PCall(int nArgs,int nResults,int errFuncIdx)
    {
        return lua_pcall(luaState,nArgs,nResults,errFuncIdx);
    }
    //consistent protected call 
    //int     CPCall(int nArgs,int nResults,int errFunc);
    //direct call
    //int     Call();        

private:
    lua_State* luaState;
};




