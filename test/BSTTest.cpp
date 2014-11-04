

#include "utility/UnitTest.h"
#include "algorithm/AVLTree.hpp"
#include "algorithm/BinarySearchArray.hpp"



int main()
{
	vector<int>		data;
	UnitTest::Instance().GenerateArray(14,-100,100,data);
	AVLTree<int>	avl;
	BinarySearchTree<int>	bst;	
	BinarySearchArray<int>	bsa;
	for(int i = 0;i < data.size();++i)
	{
		bst.Insert(data[i]);
        bsa.Insert(data[i]);
		//avl.Insert(data[i]);	
	}

    std::cout<<bst.GetMin()<<std::endl;
    std::cout<<bst.GetMax()<<std::endl;

    //std::cout<<*bsa.GetMin(bst.GetRoot())<<std::endl;
    //std::cout<<*bsa.GetMax(bst.GetRoot())<<std::endl;

    std::cout<<avl.GetMin()<<std::endl;
    std::cout<<avl.GetMax()<<std::endl;

    //UnitTest::Test(

	return 0;	
}


