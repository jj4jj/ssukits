
#include "utility/UnitTest.h"
#include "algorithm/AVLTree.h"



int main()
{
	vector<int>		data;
	UnitTest::Instance().GenerateArray(14,-100,100,data);
	AVLTree<int>	avl;
	BinarySearchTree<int>	bst;	
	BinarySearArray<int>		bsa;
	for(int i = 0;i < data.size();++i)
	{
		bst.Insert(data[i]);
		avl.Insert(data[i]);	
	}


	//UnitTest::Test();				
	return 0;	
}




