#pragma once


#include "base/stdinc.h"
#include "ucontext.h"
#include "base/Buffer.h"
#include "base/Singleton.hpp"
#include "base/Log.h"

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


class CoroutineMgr : public Singleton<CoroutineMgr>
{
public:
enum {
		DEFAULT_STACK_SIZE = 8192,	
        DEFAULT_COROUTINE_NUM = 64,
	};
private:
    Coroutine*  GetCurrent()
    {
        return current;
    }
    void        SetCurrent(Coroutine* co)
    {
        current = co;
    }
    Coroutine * Alloc()
	{
		int iID = 1;
		while(iID < iCosCount &&
			  Find(iID) != NULL)
		{
			iID++;
		}
		if(iID == iCosCount )//iCosCount >= iCosCap)
		{
			if(iCosCount >= iCosCap)
			{
                assert(iCosCount == iCosCap);
				cos = (Coroutine*)realloc(cos,2*iCosCap*sizeof(Coroutine));
				if(!cos)
				{
					//no mem more
					return NULL;	
				}

                for(int i = iCosCap;i < 2*iCosCap; ++i)
                {
                    cos[i].construct();
                }
				iCosCap *= 2;
			}
			iCosCount++;
		}
		else
		{
			assert(iID < iCosCount && iID > 0);
		}
		Coroutine & co =  cos[iID];
		co.iID = iID;
		return &co;
	}
	void Free(Coroutine* co)
	{
		co->stack.Destroy();
		co->construct();
	}
public:
 	int Init()
    {
        iCosCap = DEFAULT_COROUTINE_NUM;
        iCosCount = 1;
		cos = (Coroutine*)malloc(iCosCap*sizeof(Coroutine));
		if(!cos)
		{
			//no mem more
			return -1;
		}        
        for(int i = 0;i < iCosCap; ++i)
        {
            cos[i].construct();
        }
        Coroutine & co = cos[0];
        co.bState = Coroutine::COROUTINE_STATUS_RUNNING;
        co.from = NULL;
        co.iID = 0;        
        current = &co;
        getcontext(&(co.ctx));//just for initializing
        return 0;
    }
	~CoroutineMgr()
	{
		for(int i = 0;i < iCosCount; ++i)
		{
			Free(&(cos[i]));
		}
		free(cos);
		iCosCount = 0;
		iCosCap = 0;
	}   
public:

	static void	CoroutineFuncStub(Coroutine* co,CoroutineFunc f,void* arg)
	{
		LOG_INFO("co = %d is start , it is from co = %d",co->iID,co->from->iID);
		f(co,arg);
		co->bState = Coroutine::COROUTINE_STATUS_STOP;
		//co->iID = 0;
		LOG_INFO("co = %d is stoped , destroy it . decide next co = %d",co->iID,co->from->iID);
		CoroutineMgr::Instance().Yield(co->from->iID);
		LOG_FATAL("shouldn't be called ");
	}
	
    int	Create(CoroutineFunc f,void * arg)
	{
		Coroutine * co = Alloc();
		if(!co)
		{
			return -1;	
		}
		co->from = GetCurrent();
        if(NULL == co->stack.pBuffer)
        {
            co->stack.Create(DEFAULT_STACK_SIZE);
        }
        getcontext(&(co->ctx));
        co->ctx.uc_link = 0;
        co->ctx.uc_stack.ss_sp = co->stack.pBuffer;
        co->ctx.uc_stack.ss_size = co->stack.iCap;
        makecontext(&co->ctx, (void(*)(void))CoroutineFuncStub,3,co,f,arg);
        co->bState = Coroutine::COROUTINE_STATUS_SUSPEND;        
		LOG_INFO("co = %d is create ",co->iID);
		return co->iID;
	}
	int	Resume(int coid)
	{
		Coroutine * co = Find(coid);
		if(!co || co->bState == Coroutine::COROUTINE_STATUS_STOP )
		{
			LOG_FATAL("coid = %d pointer = %p",coid,co);
			return -1;	
		}
		Coroutine * cur = GetCurrent();
		co->from = cur;
		co->bState = Coroutine:: COROUTINE_STATUS_RUNNING;
		//make ctx
        SetCurrent(co);
		LOG_INFO("co = %d status = %d is resume from = %d",co->iID,co->bState,cur->iID);
        swapcontext(&(cur->ctx),&(co->ctx));
		return 0;
	}
	int	Yield(int coid = 0)
	{
        Coroutine * cur = GetCurrent();
        if(!cur)
        {
            return -1;
        }
		Coroutine * co = Find(coid);
		if(0 == coid)
		{
			//switch to prev
			co = cur->from;
		}
        if(!co)
        {
			LOG_FATAL("current from co is null !");
            return -1;
        }
		if(cur->bState == Coroutine::COROUTINE_STATUS_RUNNING)
		{
			cur->bState = Coroutine::COROUTINE_STATUS_SUSPEND;
		}
		co->bState = Coroutine::COROUTINE_STATUS_RUNNING;
        SetCurrent(co);
		LOG_INFO("co = %d is yield schedule next = %d for = %d",cur->iID,co->iID,coid);
        swapcontext(&(cur->ctx),&(co->ctx));
        return 0;
	}
	int	GetStatus(int coid)
	{
		Coroutine * co = Find(coid);
		if(!co)
		{
			
			return -1;
		}
		return co->bState;
	}
	Coroutine*	Find(int id)
	{
		if(id >= iCosCap)
		{
			return NULL;	
		}
		Coroutine & co =  cos[id];
		if(co.iID == id )
		{
			return &co;	
		}
		return NULL;	
	}
    void    BackTrace()
    {
        Coroutine* co = GetCurrent();
        int i = 0;
		printf("current coroutine = %d\n",co->iID);
        while(co)
        {
            printf("coroutine chain:%03d: [id=%04d,addr=0x%08lx]\n",i,co->iID,(ptrdiff_t)co);
            co = co->from;
            ++i;
        }
    }

private:
	int				iCosCap;
	int				iCosCount;
	Coroutine	*	cos;	
    Coroutine   *   current;
public:
	static void	f1(Coroutine* co,void* p)
	{
		const char * s = (const char*)p;
		printf("%s:%s\n",__FUNCTION__,s);
		printf("%s:before yield co=%d\n",__FUNCTION__,co->iID);
		CoroutineMgr::Instance().Yield();
		printf("%s:after yield co=%d\n",__FUNCTION__,co->iID);
		CoroutineMgr::Instance().BackTrace();
	}
	static void	f2(Coroutine* co,void* p)
	{
		const char * s = (const char*)p;
		printf("%s:%s\n",__FUNCTION__,s);
		int co2 = CoroutineMgr::Instance().Create(f1,(void*)"in f2 create co");		
		printf("%s:before yield co=%d\n",__FUNCTION__,co->iID);
		CoroutineMgr::Instance().Yield(co2);//equal to resume(co);
		printf("%s:after yield co=%d\n",__FUNCTION__,co->iID);
		CoroutineMgr::Instance().Resume(co2);
		CoroutineMgr::Instance().BackTrace();
	}
	void	Test()
	{
		
		int coid1 = Create(f1,(void*)"hello,world!");		
		int coid2 = Create(f2,(void*)"nihao,shijie!");		
		Resume(coid1);
		Resume(coid2);
		Resume(coid1);
		Resume(coid2);
		Resume(coid2);
		Resume(coid1);
		Resume(coid2);

		
	}

};










