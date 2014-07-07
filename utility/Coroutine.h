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
	enum {
		DEFAULT_STACK_SIZE=8192,	
	};
};


class CoroutineMgr
{
public:
    int	Create(CoroutineFunc f,void * arg);
	int	Resume(int coid);
	int	Yield(int coid);
	int	GetStatus(int coid);
	Coroutine*	Find(int id);
private:
	int				iCosCap;
	int				iCosCount;
	Coroutine*		cos;	
	int				iNextId;
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










