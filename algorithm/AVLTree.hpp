#pragma once

#include "BinarySearchTree.hpp"


template <class U>
struct  AVLNode : public BSTNode<U>
{
	int	  height;
};

template<class U,class Compare = std::less<U>  >
class AVLTree : public BinarySearchTree<U,Compare,AVLNode<U> >
{    
public:
    typedef     typename BinarySearchTree<U,Compare>::Node  Node;
    typedef     typename BinarySearchTree<U,Compare>::NodePtr       NodePtr;
    typedef     typename BinarySearchTree<U,Compare>::NodeRef       NodeRef;  
    typedef     BinarySearchTree<U,Compare,AVLNode<U> >   BST;

protected:
    
    using     BST::root;
    
public:
    inline int     GetHeight(NodePtr pNode)
    {
        if(NULL == pNode)
        {
            return 0;
        }
        else
        {
			return pNode->height;
        }
        //no this path
        return 0;
    }
	void	UpdateHeight(NodePtr pNode,int iChg);
	int     GetBalanceFactor(NodePtr pNode);    
protected:
	int RotateR(NodePtr pNode);
	//the mirror of RotateR
	int	RotateL(NodePtr pNode);
    //balance one node
	void BalanceNode(NodePtr pNode);
	virtual NodePtr Insert(NodePtr pTree,NodePtr pNode);
	virtual NodePtr Delete(NodePtr pNode) ;

};



///////////////////////////////////////////////////////////////////////////////////////


template<class U,class Compare >
void	AVLTree<U,Compare>::UpdateHeight(AVLTree<U,Compare>::NodePtr pNode,int iChg)
{
	if(0 == iChg)
	{
		return;	
	}
	pNode->height += iChg;
	while(pNode->parent)
	{
		int newHeight = MAX(GetHeight(pNode->parent->left),
									GetHeight(pNode->parent->right))+1;
		if(pNode->parent->height == newHeight)
		{
			break;
		}
		pNode->parent->height = newHeight;
	}
}
template<class U,class Compare >
int     AVLTree<U,Compare>::GetBalanceFactor(AVLTree<U,Compare>::NodePtr pNode)
{
    if(NULL == pNode)
    {
        return 0;
    }
    return GetHeight(pNode->left) - GetHeight(pNode->right);
}

template<class U,class Compare >
void AVLTree<U,Compare>::BalanceNode(AVLTree<U,Compare>::NodePtr pNode)
{
	int iFac = GetBalanceFactor(pNode);
	if(iFac > -2 && iFac < 2)
	{
		return 0;	
	}
	else if(2 == iFac)
	{
		int iChildFac =  GetBalanceFactor(pNode->left);
		if(1 == iChildFac)
		{
			RotateR(pNode);	
		}
		else if(-1 == iChildFac)
		{
			RotateL(pNode->left);
			RotateR(pNode);	
		}
		else
		{
			assert(false);	
		}
	}
	else if(-2 == iFac)
	{
		int iChildFac =  GetBalanceFactor(pNode->left);
		if(1 == iChildFac)
		{
			RotateL(pNode);	
		}
		else if(-1 == iChildFac)
		{
			RotateR(pNode->right);
			RotateL(pNode);	
		}		
		else
		{
			assert(false);	
		}
	}
	else
	{
		assert(false);
		return -1;	
	}
	return 0;
}

template<class U,class Compare >
typename BinarySearchTree<U, Compare>::NodePtr
    AVLTree<U,Compare>::Insert(AVLTree<U,Compare>::NodePtr pTree,
                               AVLTree<U,Compare>::NodePtr pNode)
{
	//		
	NodePtr pInsertNode = BST::Insert(pTree,pNode);
	assert(pInsertNode);
	NodePtr p = pInsertNode->parent;
	while(p!=NULL)
	{
		BalanceNode(p);	
		p = p->parent;	
	}
	return pInsertNode;
}

template<class U,class Compare >
typename BinarySearchTree<U, Compare>::NodePtr
    AVLTree<U,Compare>::Delete(AVLTree<U,Compare>::NodePtr pNode) 
{
	//	
	NodePtr pDeleteNode = BST::Delete(pNode);
	if(NULL == pDeleteNode)
	{
		return NULL;
	}
	while(pNode)
	{
		BalanceNode(pNode);
		pNode = pNode->parent;	
	}
	return pDeleteNode;
}

template<class U,class Compare >
int AVLTree<U,Compare>::RotateR(AVLTree<U,Compare>::NodePtr pNode)
{
	/*
				8						6
			  /  \					  /  \
			6	 11	=>			     4	  8
		  /	 \						/    / \
		4	  7				       2	7	11
	   /
	 2
	*/		
	//right rotation 
	//just move the node to next  node	position
	//node -> node->right
	//node->left -> node
	//node->left->right -> node->left
	// 6 7 8 11
	//node , node->left , node->left->right , node->right	


	//left child right child trune parent's left child	
	pNode->left->right->parent = pNode;
	pNode->left->right = pNode;
	pNode->left->parent = pNode->parent;
	if(pNode->parent)
	{
		if(IsLeftChild(pNode))
		{
			pNode->parent->left = pNode->left;	
		}
		else
		{
			pNode->parent->right = pNode->left;
		}
	}
	else
	{
		root = pNode->left;	
	}
	UpdateHeight(pNode->parent,-1);
	pNode->height -= 2;
	pNode->parent = pNode->left;
	pNode->left = pNode->left->right;
    return 0;
}


//the mirror of RotateR
template<class U,class Compare >
int	AVLTree<U,Compare>::RotateL(AVLTree<U,Compare>::NodePtr pNode)
{
	/*
			1						4
		   / \					   / \
		  2	  4					  1   5
		     / \   ===>			 / \   \
			3   5				2   3	6
				 \
				  6
	
	*/			
	
	pNode->right->left->parent = pNode;
	pNode->right->left = pNode;
	pNode->right->parent = pNode->parent;
	if(pNode->parent)
	{
		if(IsLeftChild(pNode))
		{
			pNode->parent->left = pNode->right;	
		}
		else
		{
			pNode->parent->right = pNode->right;
		}
	}
	else
	{
		root = pNode->right;	
	}
	pNode->height -= 2;
	UpdateHeight(pNode->parent,-1);
	pNode->parent = pNode->right;
	pNode->right = pNode->right->left;	
    return 0;
}

