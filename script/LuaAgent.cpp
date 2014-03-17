
#include "base/Log.h"
#include "LuaAgent.h"
//stack model
//   N -> GetTop(),  -1
//   N-1      ,      -2
//   .        ,       .
//   .                .
//   .                .
//   1               -N
// positive index : -n+N+1

//LuaAgent is a luastate helper
LuaAgent::LuaAgent():luaState(NULL)
{

}
LuaAgent::~LuaAgent()
{
    Destroy();
}
//attach a exist lua state
void  LuaAgent::Attach(lua_State* pState)
{
    Destroy();
    luaState = pState;
}
//create a lua state
int  LuaAgent::Init()
{
    luaState = luaL_newstate();
    if(NULL == luaState)
    {
        LOG_FATAL("lua new state error !");
        return -1;
    }
    //link error
    luaL_openlibs(luaState);
    LOG_TRACE("lua agent init ok .");
    return 0;
}
//
int  LuaAgent::LoadFile(const char* pszFileName)
{
    return luaL_loadfile(luaState,pszFileName);
}
//
int  LuaAgent::LoadBuffer(const char* pBuffer,size_t zBuffLen)
{
    return luaL_loadbuffer(luaState,pBuffer,zBuffLen,NULL);
}

//lua_close
void     LuaAgent::Destroy()
{
    if(!luaState)
    {
        lua_close(luaState);
        LOG_TRACE("lua agent close .");
    }
}
int      LuaAgent::OpenLib(const char * szLibName)
{
    return -1;
//    return luaL_openlib(luaState,szLibName,NULL);
}
lua_State*   LuaAgent::GetLuaState()
{
    return luaState;
}
void     LuaAgent::PushNil()
{
    lua_pushnil(luaState);
}
void     LuaAgent::PushNumber(double number)
{
    lua_pushnumber(luaState,number);
}
void     LuaAgent::PushInteger(ptrdiff_t n)
{
    lua_pushinteger(luaState,n);
}
void     LuaAgent::PushLString(const char* pBuffer,size_t zBufflen)
{
    lua_pushlstring(luaState,pBuffer,zBufflen);
}
void     LuaAgent::PushString(const char* pszString)
{
    lua_pushstring(luaState,pszString);
}
void     LuaAgent::PushBoolean(bool b)
{
    lua_pushboolean(luaState,b?1:0);
}
void     LuaAgent::PushCClosure(lua_CFunction f,int n)
{
    lua_pushcclosure(luaState,f,n);
}
const char*  LuaAgent::GetTypeName(int type)
{
    return lua_typename(luaState,type);
}
int      LuaAgent::GetType(int idx)
{
    return lua_type(luaState,idx);
}

double       LuaAgent::ToNumber(int idx)
{
    return lua_tonumber(luaState,idx);
}
ptrdiff_t    LuaAgent::ToInteger(int idx)
{
    return lua_tointeger(luaState,idx);
}
bool          LuaAgent::ToBoolean(int idx)
{
    return (lua_toboolean(luaState,idx)>0);
}
const char*  LuaAgent::ToLString(int idx,size_t * len)
{
    return lua_tolstring(luaState,idx,len);
}
lua_CFunction  LuaAgent::ToCFunction(int idx)
{
    return lua_tocfunction(luaState,idx);
}

int      LuaAgent::GetTop()
{
    return lua_gettop(luaState);
}
void     LuaAgent::SetTop(int idx)
{
    lua_settop(luaState,idx);
}
//remove a stack elment
void     LuaAgent::Remove(int idx)
{
    lua_remove(luaState,idx);
}
//move top element to a pos-1 (top dir) (lenth===)
void     LuaAgent::Insert(int idx)
{
    lua_insert(luaState,idx);
}
//move top elemtn to a pos (lenth--)
void     LuaAgent::Replace(int idx)
{
    lua_replace(luaState,idx);
}
//check stack size
int     LuaAgent::CheckStack(int sz)
{
    return lua_checkstack(luaState,sz);
}
//Pop n elements from stack
void     LuaAgent::Pop(int n)
{
    lua_pop(luaState,n);
}

void     LuaAgent::GetTable(int idx)
{
    lua_gettable(luaState,idx);
}
void     LuaAgent::GetField(int idx,const char* pszKey)
{
    lua_getfield(luaState,idx,pszKey);
}
void     LuaAgent::SetTable(int idx)
{
    lua_settable(luaState,idx);
}
void     LuaAgent::SetField(int idx,const char* pszKey)
{
    lua_setfield(luaState,idx,pszKey);
}
void     LuaAgent::GetGlobal(const char* pszKey)
{
    lua_getglobal(luaState,pszKey);
}
void     LuaAgent::SetGlobal(const char* pszKey)
{
    lua_setglobal(luaState,pszKey);
}

//protected call with a err handler
int      LuaAgent::PCall(int nArgs,int nResults,int errFuncIdx)
{
    return lua_pcall(luaState,nArgs,nResults,errFuncIdx);
}
