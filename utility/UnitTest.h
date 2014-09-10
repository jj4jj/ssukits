#pragma once

#include "base/stdinc.h"
#include "base/Singleton.hpp"

//support a simple unit test framwork ..

//1.AddTestCase	->	Function	->in	-> out	compare with expect
//2.Utility
	//GenerateNumberArray
	//GenerateText
//3.EXPECT_CHECK
//4.Run


struct TestCaseResult
{
	timeval	runningTime;
	//ok = 0
	//otherwise , ec 
	int			status;
	string		casename;
};

class UnitTest : public Singleton<UnitTest>
{
public:
	int	Init();
	#define	Test(casename,expectrval,functionv,params...) do{\
		UnitTest::Instance().BeginTest();\
		UnitTest::Instance().EndTest(casename,expectrval==functionv(##params)?0:-1);\
	}while(false)
	int		PrintReport();
	const vector<TestCaseResult>	& GetReport();
	void	BeginTest();
	void	EndTest(string casename,int stat);
public:
	template<class T>
	static int GenerateArray(int n,T minv, T maxv,vector<T> & vec );
	template<class T>
	static bool		 IsVecEqual(const vector<T> & a1,const vector<T> & a2);
private:	
	vector<TestCaseResult>	results;
	struct timeval			lastbegintime;
};


template<class T>
int UnitTest::GenerateArray(int n,T minv, T maxv,vector<T> & vec)
{
	vec.clear();
	if(maxv == minv || maxv < minv)
	{
		vec.assign(minv,n);
		return -1;
	}

	T	v;
	while(n>0)
	{
		v  = rand()%(maxv-minv+1)+minv;
		vec.push_back(v);
		n--;
	}
	return 0;	
}
template<class T>
bool		 UnitTest::IsVecEqual(const vector<T> & a1,const vector<T> & a2)
{
	if(a1.size() != a2.size())
	{
		return false;
	}
	for(int i = 0;i < a1.size() ;++i)
	{
		if(a1[i]!=a2[i])
		{
			return false;
		}
	}
	return true;
}
