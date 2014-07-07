#pragma once




typedef void	CoroutineFunc(int coid,void* arg);

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
};


class CoroutineMgr
{
public:
enum {
		DEFAULT_STACK_SIZE=8192,	
	};
public:
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
				cos = realloc(cos,2*iCosCap);
				if(!cos)
				{
					//no mem more
					return NULL;	
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
    int	Create(CoroutineFunc f,void * arg)
	{
		Coroutine * co = Alloc();
		if(!co)
		{
			return -1;	
		}
		co->iID = iNextId;
		co->stack.Create(DEFAULT_STACK_SIZE);
		co->from = GetCurrent();
		//co.ctx = 
		//set co.ctx.start = f.
		//set co.ctx.
		return co->iID;
	}
	int	Resume(int coid)
	{
		Coroutine * co = Find(coid);
		if(!co)
		{
			return -1;	
		}
		Coroutine * cur = GetCurrent();
		co->from = cur;
		co->bState = Coroutine:: COROUTINE_STATUS_RUNNING
		//make ctx
		
		return 0;
	}
	int	Yield(int coid)
	{
		Coroutine * co = Find(coid);
		if(0 == coid)
		{
			//switch to prev
		}
		co->bState = Coroutine::COROUTINE_STATUS_SUSPEND;
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
		Coroutine * co =  cos[id];
		if(co.iID == id)
		{
			return &co;	
		}
		return NULL;	
	}
private:
	int				iCosCap;
	int				iCosCount;
	Coroutine	*	cos;	
public:
	static void	f1(int,void* p)
	{
		const char * s = (const char*)p;
		printf("%s:%s\n",__FUNCTION__,s);
		printf("%s:before yield\n",__FUNCTION__);
		yield();
		printf("%s:after yield\n",__FUNCTION__);
	}
	static void	f2(int,void* p)
	{
		const char * s = (const char*)p;
		printf("%s:%s\n",__FUNCTION__,s);
		Coroutine co = CoroutineMgr::Instance().Create(f1,"in f2 create co");		
		printf("%s:before yield\n",__FUNCTION__);
		CoroutineMgr::Instance().yield(co);//equal to resume(co);
		printf("%s:after yield\n",__FUNCTION__);
	}
	void	Test()
	{
		
		int coid1 = Create(f1,"hello,world!");		
		int coid2 = Create(f2,"nihao,shijie!");		
		printf("co1 status %d\n",GetStatus(coid1));
		printf("co1 status %d\n",GetStatus(coid2));
		Resume(coid1);
		printf("co1 status %d\n",GetStatus(coid1));
		Resume(coid2);
		printf("co1 status %d\n",GetStatus(coid2));
	}

};










