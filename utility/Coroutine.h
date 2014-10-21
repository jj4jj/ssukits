#pragma once

#include "base/stdinc.h"
#include "ucontext.h"
#include "base/Buffer.h"
#include "base/Singleton.hpp"

struct Coroutine;
typedef void	CoroutineFunc(Coroutine* co,void* arg);

struct	Coroutine
{
	int		iID;
	uint8_t bState;
	//context			
	Buffer		stack;
	//context
	ucontext_t	ctx;
	Coroutine	*from;
	enum
	{
        COROUTINE_STATUS_INVALID = 0,
		COROUTINE_STATUS_SUSPEND = 1,
		COROUTINE_STATUS_RUNNING = 2,
		COROUTINE_STATUS_STOP    = 3,
	};
    void construct()
    {
        iID = 0;
        bState = COROUTINE_STATUS_INVALID;
        from = NULL;        
    }
};


////////////////////////////////////////////////////////////////////////////////////
//USAGE
//1.Init
//Create Corotine
//--

class CoroutineMgr : public Singleton<CoroutineMgr>
{
private:
    CoroutineMgr(){}
	~CoroutineMgr();
    DeclareSingltonSupport(CoroutineMgr)
public:
enum {
		DEFAULT_STACK_SIZE = 8192,	
        DEFAULT_COROUTINE_NUM = 128,
	};
public:
 	int Init(int    iMaxCoroutineNum = DEFAULT_COROUTINE_NUM);
public:
	static void	CoroutineFuncStub(Coroutine* co,CoroutineFunc f,void* arg);
    int	Create(CoroutineFunc f,void * arg);	
	int	Resume(int coid,int retv = 0);
	int	Yield(int coid = 0);
	int	GetStatus(int coid);
	Coroutine*	Find(int id);
    void        BackTrace();
private:
    Coroutine*  GetCurrent();
    void        SetCurrent(Coroutine* co);
    Coroutine * Alloc();
	void        Free(Coroutine* co);
private:
	int				iCosCap;
	int				iCosCount;
	Coroutine	*	cos;	
    Coroutine   *   current;
    volatile    int   retval;
};


