#pragma once

#include "base/stdinc.h"


template <class U>
struct  BSTNode
{
    BSTNode<U>* left,*right,*parent;
    //////////////////////////
    U   data;
};
template <class U , class Compare = std::less<U> ,class NodeType = BSTNode<U> >
class BinarySearchTree
{
public:
    typedef     NodeType        Node;
    typedef     NodeType*       NodePtr;
    typedef     NodeType&       NodeRef;
	Compare	    comp;

protected:
    NodePtr   root;

protected:
    inline    void    SetRoot(NodePtr _root)
    {
        root = _root;
    }
    inline NodePtr  GetRoot()
    {
        return root;
    }
    inline bool    EQ(const U& u1,const U& u2)
    {
        return !comp(u1,u2) && !(u2,u1);
    }
    inline bool    LT(const U& u1,const U& u2)
    {
        return comp(u1,u1);
    }
    NodePtr   Alloc()
    {
        NodePtr p = new Node();
        ///////////////////////////
        return p;
    }
    void      Free(NodePtr & p)
    {
        delete p;
        p = NULL;        
    }
	///////////////////////////////////////////////
    inline bool    IsLeftChild(NodePtr pNode)
    {
        return (pNode->parent->left == pNode);
    }
    NodePtr GetUncle(NodePtr pNode)
    {
        if(NULL == pNode)
        {
            return NULL;
        }
        if(IsLeftChild(pNode))
        {
            return pNode->parent->right;
        }
        else
        {
            return pNode->parent->left;
        }
    }

public:
    BinarySearchTree():root(NULL){}    
public:    
	NodePtr	GetMin(NodePtr pNode);
	NodePtr	GetMax(NodePtr pNode);
    int     Destroy(NodePtr pTree);
protected:
    virtual NodePtr     Delete(NodePtr pNode);	
    virtual NodePtr     Insert(NodePtr pTree,NodePtr pNode);
public:
	int		Insert(const U & u);
	int		Remove(const U & u);
	U *		Find(const U & u);
    void    GetSortedList(vector<U> & vec);
};



////////////////////////////////////////////////////////////////////

template <class U , class Compare ,class NodeType  >
typename BinarySearchTree<U,Compare,NodeType>::NodePtr	BinarySearchTree<U,Compare,NodeType>::GetMin(NodePtr pNode)
{
	if(NULL == pNode ||
	    NULL == pNode->left)
	{
		return pNode;	
	}
	return GetMin(pNode->left);
}
template <class U , class Compare ,class NodeType  >
typename BinarySearchTree<U,Compare,NodeType>::NodePtr	
    BinarySearchTree<U,Compare,NodeType>::GetMax(typename BinarySearchTree<U,Compare,NodeType>::NodePtr pNode)
{
	if(NULL == pNode ||
	   NULL == pNode->right)
	{
		return pNode;	
	}
	return GetMax(pNode->right);
}
	
template <class U , class Compare ,class NodeType  >
int     BinarySearchTree<U,Compare,NodeType>::Destroy(typename BinarySearchTree<U,Compare,NodeType>::NodePtr pTree)
{
	if(NULL == pTree)
	{
		return -1;	
	}
	if(pTree->left)
	{
		Destroy(pTree->left);	
	}
	if(pTree->right)
	{
		Destroy(pTree->right);
	}
	Free(pTree);
	return 0;
}

template <class U , class Compare ,class NodeType  >
typename BinarySearchTree<U,Compare,NodeType>::NodePtr   
    BinarySearchTree<U,Compare,NodeType>::Delete(typename BinarySearchTree<U,Compare,NodeType>::NodePtr pNode)
{
	assert(pNode);
    NodePtr pReplaceNode = GetMin(pNode->right);        
	if(pNode == NULL)
	{
		pReplaceNode = GetMax(pNode->left);
		if(pReplaceNode)
		{
			//will delete
			if(IsLeftChild(pReplaceNode))
			{
				pReplaceNode->parent->left = pReplaceNode->left;		
			}
			else
			{
				pReplaceNode->parent->right = pReplaceNode->left;		
			}
			if(pReplaceNode->left)
			{
				pReplaceNode->left->parent = pReplaceNode->parent;
			}			
		}
	}
	else
	{
		//will delete
		if(IsLeftChild(pReplaceNode))
		{
			pReplaceNode->parent->left = pReplaceNode->right;		
		}
		else
		{
			pReplaceNode->parent->right = pReplaceNode->right;		
		}
		if(pReplaceNode->right)
		{
			pReplaceNode->right->parent = pReplaceNode->parent;
		}
	}

	pReplaceNode->parent = pNode->parent;
	pReplaceNode->height = pNode->height;
	if(pNode->parent)
	{
		if(IsLeftChild(pNode))
		{
			pNode->parent->left = pReplaceNode;	
		}
		else
		{
			pNode->parent->right = pReplaceNode;	
		}
	}
    return pReplaceNode;
}


template <class U , class Compare ,class NodeType  >    
typename  BinarySearchTree<U,Compare,NodeType>::NodePtr
    BinarySearchTree<U,Compare,NodeType>::Insert(typename BinarySearchTree<U,Compare,NodeType>::NodePtr pTree,
                                                 typename BinarySearchTree<U,Compare,NodeType>::NodePtr pNode)
{        
	if( EQ(pNode->u,pTree->data))
	{
		return NULL;	
	}
    else if( LT(pNode->u,pTree->data) )
    {
        if(pTree->left != NULL)
        {
            return Insert(pTree->left,pNode);
        }
        else
        {
            //conserder the thread safe ?
            pTree->left = pNode;
            pNode->parent = pTree;
            return pTree;
        }
    }
    else
    {
        if(pTree->right != NULL)
        {
            return Insert(pTree->right,pNode);
        }
        else
        {
            pTree->right = pNode;
            pNode->parent = pTree;
            return pTree;
        }
    }
    //no this path
    return NULL;            
}

///////////////////////////////////////////////////////////////////////////

template <class U , class Compare ,class NodeType  >
int		BinarySearchTree<U,Compare,NodeType>::Insert(const U & u)
{
    NodePtr p = Alloc();
    if(NULL == p)
    {
        return -1;
    }
    p->data = u;
    if(NULL == Insert(root,p))
    {
        Free(p);
		return 1;
    }
    return 0;
}

template <class U , class Compare ,class NodeType  >
int		BinarySearchTree<U,Compare,NodeType>::Remove(const U & u)
{
    ///
    NodePtr p = Find(u);
    if(NULL == p)
    {
        return -1;
    }        
    NodePtr pNewNode = Delete(p);        
	if(pNewNode != NULL)
	{
		Free(p);  		
		return 0;
	}
	return -1; 
}

template <class U , class Compare ,class NodeType  >
U *		BinarySearchTree<U,Compare,NodeType>::Find(const U & u)
{
    NodePtr p = root;
    while(p != NULL && ! EQ(u,p->data))
    {            
        if(LT(u,p->data))
        {
            p = p->left;
        }
        else
        {
            p = p->right;
        }
    }
    return p;
}

template <class U , class Compare ,class NodeType  >
void    BinarySearchTree<U,Compare,NodeType>::GetSortedList(vector<U> & vec)
{
	vec.clear();
	std::stack<NodePtr>  history;
	Node* p = root;
	history.push(root);
	while(!history.empty())
	{
		Node* p = history.top();
		if(p->left)
		{
			p = p->left;
			history.push(p);
		}
		else if(p->right)
		{
			vec.push_back(p);
			history.pop();
			p = p->right;
			history.push(p);
		}
		else
		{
			vec.push_back(p);
			history.pop();
		}
	}
}


