#pragma once


template <class U>
class RBTree
{
public:    
    struct  Node;
    enum    NodeColor
    {
        NC_RED = 0,
        NC_BLOCK,
    };
    struct  Node
    {
        Node* left,*right;
        NodeColor   nc;         
        //////////////////
        U   data;
    };
protected:
    
public:
	int		Insert(const U & u);
	int		Remove(const U & u);
	U *		Find(const U & u);	
};

