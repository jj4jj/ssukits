#pragma once




typedef void	CoroutineFunc(Coroutine co,void* arg);


struct	CoroutineData
{
	int iID;
	//context			
	Buffer	stack;
	//context
	ucontext_t	ctx;
};

class Coroutine
{
friend class CoroutineMgr;
protected:
	Coroutine(CoroutineData* p):pData(p){}
public:
	//return val equal to param is ok . 
    int     Yield(int ret);
	int		Switch(Coroutine co,int ret);
	//return 0 if cf is over
	//return the param of Yield and Switch
    int     Start(CoroutineFunc cf);
    int     Resume();
private:
	CoroutineData*	pData;
};



class CoroutineMgr : Singleton<CoroutineMgr>
{
public:
    Coroutine	Create();
	Coroutine*	Find(int id);
	void		Destroy(Coroutine co);
private:
    int				iNextCOID;
	int				iCosCap;
	int				iCosCount;
	CoroutineData*  cos;	
public:
	static  Tf(void* p)
	{
		for(int i = 0;i < 5;++i)
		{
			printf("Tf:%d\n",i);
			CoroutineMgr::Instance().yield(5-i-1);	
		}
	}
	
	void	Usage()
	{
		
		Coroutine co = CoroutineMgr::Instance().Create();		
		int t = co.Start(Tf);
		for(int i = 0;i < t; ++i)
		{
			printf("Usage:%d",i);
			co.Resume();	
		}
		co.Start(Tf);
		co.Destroy();
		
	}

};










