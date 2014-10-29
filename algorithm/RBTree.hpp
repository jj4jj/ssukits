#pragma once

#include "BinarySearchTree.h"




//todo not complete

template<class U>
struct RBTNode : public BSTNode<U>
{
    enum    NodeColor
    {
        NC_RED = 0,
        NC_BLACK,
    };
    NodeColor   nc;    
};

template < class U , class Compare = std::less<U> >
class RBTree :  public BinarySearchTree<U,Compare,RBTNode<U> >
{
public:
    using     typename BinarySearchTree<U,Compare>::NodePtr;
    using     typename BinarySearchTree<U,Compare>::NodeRef;  
    typedef   BinarySearchTree<U,Compare>   BST;
protected:
    
    using     BinarySearchTree<U,Compare>::root;
        
protected:
    void      BalanceNode(NodePtr pNode)
	{
        if(NULL == pNode)
        {
            return;
        }
        NodePtr pParent = pNode->parent;
        NodePtr pUncle = GetUncle(pNode);
        
        if(NodeType::NodeColor::NC_RED == GetNodeColor(pParent) &&
          NULL != pUncle )
        {
            if(NodeType::NodeColor::NC_RED == GetNodeColor(pUncle) )
            {
                pParent->parent->nc = NodeType::NodeColor::NC_RED;
                pUncle->nc = NodeType::NodeColor::NC_BLACK;
                pParent->nc = NodeType::NodeColor::NC_BLACK;
                return BalanceNode(pParent->parent);
            }
//            else
//            {
            //if pparent is left child
                //if pnode is left 
                //if pnode is right
            //if pparent is right child
                //...
                //...
            if(IsLeftChild(pParent)
            {
                if(IsLeftChild(pNode))
                {
                    RotateR(pParent);
                }
                else
                {
                    RotateL(pParent);
                    RotateR(pParent);
                }
            }
            else
            {
                if(IsLeftChild(pNode))
                {
                    RotateL(pParent);
                }
                else
                {
                    RotateR(pParent);
                    RotateL(pParent);                    
                }
            }
//            }
        }
	}
    inline int GetNodeColor(NodePtr pNode)
    {
        if(NULL == pNode)
        {
            return NodeType::NodeColor::NC_BLACK;
        }
        return pNode->nc;
    }
    //insert the pNode into the tree
    //return the pNode if success
	virtual NodePtr Insert(NodePtr pTree,NodePtr pNode)
	{
        NodePtr pParent =  BST::Insert(root,pNode);
        if(NULL == pParent)
        {
            return NULL;
        }
        ////////////////////////////////
        if(BalanceNode(pNode))
        {
            return pNode;
        }
        return NULL;        
	}
    //delete pNode from the tree
    //return the replace node
	virtual NodePtr Delete(NodePtr pNode) 
	{

	}
    //
	int RotateR(NodePtr pNode)
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
	int	RotateL(NodePtr pNode)
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
    
};

