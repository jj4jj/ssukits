

#include "utility/UnitTest.h"
#include "algorithm/AStar.h"


int main()
{

    UnitTest::Init();
    UnitTest::TestFunc("min",1,std::min,1,2);
    UnitTest::TestExpr("te",std::min(1,2) == 1);

    return 0;
}

