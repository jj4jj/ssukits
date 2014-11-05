
#include "utility/UnitTest.h"
#include "algorithm/BinarySearchArray.hpp"



int main()
{
	vector<int>		data;
    data.reserve(128);
	UnitTest::Instance().GenerateArray(14,-100,100,data);


    UnitTest::PrintArray(data);
    size_t n = data.size();
    BlockArray<int> ba(&data[0],n,data.capacity());

	BinarySearchArray<int,BlockArray<int> >	bsa(ba);
    bsa.Insert(2352);
    bsa.Insert(2552);
    bsa.Insert(2552,true);
    bsa.Insert(2552,true);
    bsa.Insert(2652);

    bsa.Insert(2652,true);
    bsa.Insert(2652,true);
    bsa.Insert(2652,true);
    //
    bsa.Insert(2652,true);

    bsa.Remove1st(2352);
    bsa.RemoveAll(2652);
    bsa.RemoveAll(2552);
    
    UnitTest::PrintArray(&data[0],n);

	return 0;	
}




