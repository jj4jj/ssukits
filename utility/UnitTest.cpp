
#include "UnitTest.h"
#include "base/Log.h"
int	UnitTest::Init()
{
	srand(time(NULL));	
	results.clear();
}


int	UnitTest::PrintReport()
{
	LOG_INFO("UnitTest Results as follow :");
	LOG_INFO("casename	\t\t status(0:success)	\t\t time(s.us)");
	for(int i = 0;i < (int)results.size() ; ++i)
	{
		LOG_INFO("%s	\t\t %d(%s)	\t\t %ds.%dus",
				results[i].casename.c_str(),
				results[i].status,
				results[i].status == 0?"success":"fail",
				results[i].runningTime.tv_sec,
				results[i].runningTime.tv_usec);
	}

}
inline const vector<TestCaseResult>	& UnitTest::GetReport()
{
	return results;	
}
inline void	UnitTest::BeginTest()
{
		gettimeofday();	
}
inline void	UnitTest::EndTest(string casename,int stat)
{
		time_val	endtime;
		gettimeofday(&endtime);
		TestCaseResult	tcr;
		tcr.runningTime.tv_sec = endtime.tv_sec - lastbegintime.tv_sec;
		tcr.runningTime.tv_usec = endtime.tv_usec - lastbegintime.tv_usec;
		tcr.casename = casename;
		tcr.status = stat;
		results.push_back(tcr);
}
