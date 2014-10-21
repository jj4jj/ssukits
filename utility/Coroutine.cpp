#include "base/Log.h"
#include "Coroutine.h"

#if 1
int CoroutineMgr::Init(int iInitMaxCoroutineNum)
{
    iCosCap = iInitMaxCoroutineNum;
    iCosCount = 1;
    retval = 0;
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
    LOG_INFO("init coroutine mgr ok init max = %d",iInitMaxCoroutineNum);
    return 0;
}
CoroutineMgr::~CoroutineMgr()
{
	for(int i = 0;i < iCosCount; ++i)
	{
		Free(&(cos[i]));
	}
	free(cos);
	iCosCount = 0;
	iCosCap = 0;
} 
#endif


#if 1
Coroutine* CoroutineMgr::GetCurrent()
{
    return current;
}

void        CoroutineMgr::SetCurrent(Coroutine* co)
{
    current = co;
}

Coroutine * CoroutineMgr::Alloc()
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
            LOG_INFO("coroutine count = %d reach max = %d will expand double",iCosCount,iCosCap);
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
void CoroutineMgr::Free(Coroutine* co)
{
	co->stack.Destroy();
	co->construct();
}
  
void	CoroutineMgr::CoroutineFuncStub(Coroutine* co,CoroutineFunc f,void* arg)
{
	LOG_INFO("co = %d is start , it is from co = %d",co->iID,co->from->iID);
	f(co,arg);
	co->bState = Coroutine::COROUTINE_STATUS_STOP;
	//co->iID = 0;
	LOG_INFO("co = %d is stoped , destroy it . decide next co = %d",co->iID,co->from->iID);
	CoroutineMgr::Instance().Yield(co->from->iID);
	LOG_FATAL("shouldn't be called ");
}
#endif


#if 1


int	CoroutineMgr::Create(CoroutineFunc f,void * arg)
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
int	CoroutineMgr::Resume(int coid,int retv)
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
    retval = retv;
    swapcontext(&(cur->ctx),&(co->ctx));
	return 0;
}
int	CoroutineMgr::Yield(int coid )
{
    Coroutine * cur = GetCurrent();
    if(!cur)
    {
		LOG_FATAL("current co is null !");
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
    return retval;
}
int	CoroutineMgr::GetStatus(int coid)
{
	Coroutine * co = Find(coid);
	if(!co)
	{
		
		return -1;
	}
	return co->bState;
}
Coroutine*	CoroutineMgr::Find(int id)
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
void    CoroutineMgr::BackTrace()
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
#endif

