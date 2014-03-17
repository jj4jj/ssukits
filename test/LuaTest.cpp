#include "script/LuaBridge.hpp"
#include "script/LuaAgent.h"


#include "base/stdinc.h"

using namespace std;

class A
{
public:    
    A(int i):_i(i){
        cout<<"contruct integer:"<<_i<<endl;
    }
    ~A()
    {
        cout<<"destruct integer:"<<_i<<endl;
    }
    void print()
    {
        cout<<"print interger:"<<_i<<endl;
    }
    virtual int f()
    {
        cout<<"virutal function A::f()"<<endl;
        return _i;
    }
public:
    int _i;
    static int j;
};
int A::j = -1;
void gFunc()
{
    cout<<"gFunc()"<<endl;
}
class B:public A
{
public:
    B():A(0){}
    virtual int f()
    {
        cout<<"virutal function B::f()"<<endl;
        return _i*_i;
    }    
};
int g_Integer = 20;


int main()
{
    LuaAgent la;
    la.Init();
    using namespace luabridge;
    getGlobalNamespace(la.GetLuaState())
    .beginNamespace("test")
        .beginClass<A>("CA")
            .addConstructor<void (*)(int)>()
            .addFunction("print",&A::print)
            .addData("i",&A::_i)
            .addFunction("f",&A::f)
            .addStaticData ("j", &A::j)
         .endClass() 
         .deriveClass<B,A>("CB")
            .addConstructor<void (*)(void)>()
            .addFunction("f",&B::f)
         .endClass()
         .addFunction("gFunc",gFunc)
         .addVariable ("g_Integer",&g_Integer)
     .endNamespace();
    B b;
    b._i = 30;
    //luabridge::setGlobal(la.GetLuaState(),b,"b");
    //to lua
    //la.LoadFile("TestLua.lua");
    //from lua
        
    
    return 0;
}





