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
		COROUTINE_STATUS_SUSPEND = 1,
		COROUTINE_STATUS_RUNNING = 2,
		COROUTINE_STATUS_STOP    = 3,
	};
    void construct()
    {
        iID = 0;
        bState = 0;
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
public:
enum {
		DEFAULT_STACK_SIZE = 8192,	
        DEFAULT_COROUTINE_NUM = 64,
	};
public:
 	int Init();
	~CoroutineMgr();
public:
	static void	CoroutineFuncStub(Coroutine* co,CoroutineFunc f,void* arg);
    int	Create(CoroutineFunc f,void * arg);	
	int	Resume(int coid);
	int	Yield(int coid = 0);
	int	GetStatus(int coid);
	Coroutine*	Find(int id);
    void    BackTrace();
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
};


