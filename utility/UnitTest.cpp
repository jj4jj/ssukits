
#include "UnitTest.h"
#include "base/Log.h"
int	UnitTest::Init()
{
	srand(time(NULL));	
	results.clear();
	return 0;
}


int	UnitTest::PrintReport()
{
	LOG_INFO("UnitTest Results as follow :");
	LOG_INFO("casename	\t\t status(0:success)	\t\t time(s.us)");
	for(int i = 0;i < (int)results.size() ; ++i)
	{
		LOG_INFO("%s	\t\t %d(%s)	\t\t %ds.%dus in file(%s:%d) func(%s) scope(%s)",
				results[i].casename.c_str(),
				results[i].status,
				results[i].status == 0?"success":"fail",
				results[i].runningTime.tv_sec,
				results[i].runningTime.tv_usec,
				results[i].scopefile.c_str(),
				results[i].line,
				results[i].testfunc.c_str(),
				results[i].scopefunc.c_str());
	}
	return 0;

}
inline const vector<TestCaseResult>	& UnitTest::GetReport()
{
	return results;	
}
inline void	UnitTest::BeginTest()
{
		gettimeofday(&lastbegintime,NULL);	
}
inline void	UnitTest::EndTest(string casename,int stat,const char* pszTestFuncName,const char* pszFile,const char* pszFunction,int iLine)
{
		timeval	endtime;
		gettimeofday(&endtime,NULL);
		TestCaseResult	tcr;
		tcr.runningTime.tv_sec = endtime.tv_sec - lastbegintime.tv_sec;
		tcr.runningTime.tv_usec = endtime.tv_usec - lastbegintime.tv_usec;
		tcr.casename = casename;
		tcr.status = stat;
        tcr.scopefile = pszFile;
        tcr.scopefunc = pszFunction;
        tcr.testfunc = pszTestFuncName;
        tcr.line = iLine;
		results.push_back(tcr);
}
