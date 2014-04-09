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
<<<<<<< HEAD
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
=======
        Node* left,*right,*parent;
		int	  height;
        //////////////////////////
        U   data;
    };
private:
    Node*   root;
	Compare	comp;
protected:    
    Node*   Alloc()
>>>>>>> ffb804a3de66b8794d53bb63a12b28ec59a73ba9
    {
        NodePtr p = new Node();
        memset(p,0,sizeof(Node));
        p->left = p->right = p->parent = NULL;
		p->height = 0;
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
    void    GetSortedList(vector<U> & vec)
    {
        vec.clear();
        stack<NodePtr>  history;
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
<<<<<<< HEAD
	else
	{
		//will delete
		if(IsLeftChild(pReplaceNode))
=======
	inline int     GetHeight(Node* pNode)
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
	void	UpdateHeight(Node* pNode,int iChg)
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
    int     Destroy(Node* pTree)
    {
		if(NULL == pTree)
>>>>>>> ffb804a3de66b8794d53bb63a12b28ec59a73ba9
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
<<<<<<< HEAD
			pNode->parent->left = pReplaceNode;	
		}
		else
		{
			pNode->parent->right = pReplaceNode;	
=======
			pReplaceNode = GetMax(pNode->left);
			if(pReplaceNode)
			{
				//will delete
				UpdateHeight(pReplaceNode,-1);
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
			UpdateHeight(pReplaceNode,-1);
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
>>>>>>> ffb804a3de66b8794d53bb63a12b28ec59a73ba9
		}
	}
    return pReplaceNode;
}

<<<<<<< HEAD

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
=======
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
	
    virtual Node*     Insert(Node * pTree,Node* pNode)
    {        
		if( equal(pNode->u,pTree->data))
		{
			return NULL;	
		}
        else if( comp(pNode->u,pTree->data) )
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
				UpdateHeight(pNode,1);
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
				UpdateHeight(pNode,1);
                return pTree;
            }
>>>>>>> ffb804a3de66b8794d53bb63a12b28ec59a73ba9
        }
    }
    else
    {
        if(pTree->right != NULL)
        {
            return Insert(pTree->right,pNode);
        }
<<<<<<< HEAD
        else
        {
            pTree->right = pNode;
            pNode->parent = pTree;
            return pTree;
=======
        p->data = u;
        if(NULL == Insert(root,p))
        {
            Free(p);
			return 1;
>>>>>>> ffb804a3de66b8794d53bb63a12b28ec59a73ba9
        }
    }
<<<<<<< HEAD
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
=======
	int		Remove(const U & u)
	{
        ///
        Node* p = Find(u);
        if(NULL == p)
        {
            return -1;
        }        
        Node* pNewNode = Delete(p);        
		if(pNode != NULL)
		{
			Free(p);  		
			return 0;
		}
		return -1; 
>>>>>>> ffb804a3de66b8794d53bb63a12b28ec59a73ba9
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




