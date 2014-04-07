#pragma once


template <class U , class Compare = std::less<U> >
class BinarySearchTree
{
public:    
    struct  Node;
    struct  Node
    {
        Node* left,*right,*parent;
		int	  height;
        //////////////////////////
        U   data;
    };
private:
    Node*   root;
protected:    
    Node*   Alloc()
    {
        Node* p = new Node();
        p->left = p->right = p->parent = NULL;
        return p;
    }
    void    Free(Node* & p)
    {
        delete p;
        p = NULL;        
    }
	///////////////////////////////////////////////
    inline bool    IsLeftChild(Node* pNode)
    {
        return (pNode->parent->left == pNode);
    }
	Node*	GetMin(Node* pNode)
	{
		if(NULL == pNode ||
		    NULL == pNode->left)
		{
			return pNode;	
		}
		return GetMin(pNode->left);
	}
	Node*	GetMax(Node* pNode)
	{
		if(NULL == pNode ||
		   NULL == pNode->right)
		{
			return pNode;	
		}
		return GetMax(pNode->right);
	}
    int     Destroy(Node* pTree)
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
		Free(p);
		return 0;
    }
public:
    BinarySearchTree():root(NULL){}    
public:
    virtual Node*   Delete(Node* pNode)
    {
		assert(pNode);
        Node* pReplaceNode = GetMin(pNode->right);        
		if(NULL == pReplaceNode ||
		   pNode == pReplaceNode)
		{
			pReplaceNode = GetMax(pNode->left);
		}

		if(NULL != pReplaceNode)
		{
			//

		}

        Free(pNode);
        return pReplaceNode;
    }
    virtual Node*     Insert(Node * pTree,Node* pNode)
    {        
        if( comp(pNode->u,pTree->data) )
        {
            if(pTree->left != NULL)
            {
                return Insert(pTree->left);
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
                return Insert(pTree->right);
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

public:
	int		Insert(const U & u)
	{
        Node* p = Alloc();
        if(NULL == p)
        {
            return -1;
        }
        p->data = u;
        if(Insert(root,p))
        {
            Free(p);
        }
        return 0;
    }
	int		Remove(const U & u)
	{
        ///
        Node* p = Find(u);
        if(NULL == p)
        {
            return -1;
        }        
        return Delete(p)!=NULL;        
    }
	U *		Find(const U & u)
	{
        Node* p = root;
        Compare comp;
        while(p!=NULL && !equal(u,p->data))
        {            
            if(comp(u,p->data))
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
};




