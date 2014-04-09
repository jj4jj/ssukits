#pragma once


#include "BinarySearchTree.h"

template<class U , class Compare = std::less<U>  >
class AVLTree:public BinarySearchTree<U,Compare>
{
public:
	int     GetBalanceFactor(Node* pNode)
    {
        if(NULL == pNode)
        {
            return 0;
        }
        return GetHeight(pNode->left) - GetHeight(pNode->right);
    }
    
protected:
	void BalanceNode(Node* pNode)
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
	Node* Insert(Node* pTree,Node* pNode)
	{
		//		
		Node* pInsertNode = BinarySearchTree::Insert(pTree,pNode);
		assert(pInsertNode);
		Node* p = pInsertNode->parent;
		while(p!=NULL)
		{
			BalanceNode(p);	
			p = p->parent;	
		}
		return pInsertNode;
	}
	Node* Delete(Node* pNode) 
	{
		//	
		Node* pDeleteNode = BinarySearchTree::Delete(pNode);
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
	int RotateR(Node* pNode)
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
	}
	//the mirror of RotateR
	int	RotateL(Node* pNode)
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
	}

};

