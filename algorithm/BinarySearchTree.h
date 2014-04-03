#pragma once


template <class U , class Compare = std::less<U> >
class BinarySearchTree
{
public:    
    struct  Node;
    struct  Node
    {
        Node* left,*right;
        //////////////////
        U   data;
    };
private:
    Node*   root;
protected:    
    Node*   Alloc();
    void    Free(Node* p);
    int     Insert(Node* pTree,Node* pNode);
    int     Destroy(Node* pTree);
    int     Delete(Node* pNode);
public:
    BinarySearchTree():root(NULL);    
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
        return Delete(p);        
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




