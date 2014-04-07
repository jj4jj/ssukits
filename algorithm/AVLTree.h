#pragma once


template <class U , class Compare = std::less<U> >
class AVLTree
{
public:    
    struct  Node;
    struct  Node
    {
        Node* left,*right,*parent;
        //////////////////
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
    Node*     RawInsert(Node * pTree,Node* pNode)
    {        
        if( comp(pNode->u,pTree->data) )
        {
            if(pTree->left != NULL)
            {
                return RawInsert(pTree->left);
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
                return RawInsert(pTree->right);
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
    int     GetBalanceFactor(Node* pNode)
    {
        if(NULL == pNode)
        {
            return 0;
        }
        return GetHeight(pNode->left) - GetHeight(pNode->right);
    }
    int     GetHeight(Node* pNode)
    {
        if(NULL == pNode)
        {
            return 0;
        }
        else
        {
            return 1 + MAX(GetHeight(pNode->left,pNode->right));
        }
        //no this path
        return 0;
    }
    inline bool    IsLeftChild(Node* pNode)
    {
        return (pNode->parent->left == pNode);
    }
    void    LLRotate(Node* pGrand,Node* pSon)
    {
        /*  3(grand)            2
           2        =>        1   3
          1(son)

        */
        //set son's parent position
        pSon->parent->parent = pGrand->parent;
        if(IsLeftChild(pGrand))
        {
            pGrand->parent->left = pSon->parent;
        }
        else
        {
            pGrand->parent->right = pSon->parent;
        }
        //
        pSon->parent->right = pGrand;
        pGrand->parent = pSon->parent;
        pGrand->left = NULL;        
    }
    void    LRRotate(Node* pGrand,Node* pSon)
    {
        /*  4               3
          2        =>     2   4
            3

        */
        pSon->parent->parent = pSon;
        pSon->parent->right = NULL;
        pSon->parent = pGrand->parent;
        if(IsLeftChild(pGrand))
        {
            pGrand->parent->left = pSon;
        }
        else
        {
            pGrand->parent->right = pSon;
        }
        pGrand->parent = pSon;
        pSon->right = pGrand;
        pGrand->left  = NULL;
    }
    void    RLRotate(Node* pGrand,Node* pSon)
    {
        /*
            3                       5
              5         =>        3   6
                6
        */
        pSon->parent->parent = pGrand->parent;
        if(IsLeftChild(pGrand))
        {
            pGrand->parent->left = pSon->parent;
        }
        else
        {
            pGrand->parent->right = pSon->parent;
        }

        pSon->parent->left = pGrand;
        pGrand->parent = pSon->parent;
        pGrand->right = NULL;
                        
    }
    void    RRRotate(Node* pGrand,Node* pSon)
    {
        /*
             3                      6
                7       =>       3     7
             6
        */
        pSon->parent->left = NULL;
        pSon->parent->paren = pSon;     
        pSon->parent = pGrand;
        if(IsLeftChild(pGrand))
        {
            pGrand->parent->left = pSon;
        }
        else
        {
            pGrand->parent->right= pSon;
        }
        pSon->left = pGrand;
        pGrand->parent = pSon;
        pGrand->right = NULL;
    }
    void    BalanceNode(Node* pNode)
    {
       int iGrandBalnceFactor = GetBalanceFactor(pNode);
       //case : -1, 2 , 1: 2
       //       -1,-2 , 1,-2
       if(iGrandBalnceFactor > -2 &&
          iGrandBalnceFactor < 2)
       {
            return ;
       }
       if(2 == iGrandBalnceFactor)
       {
            if(pNode->left->left && NULL == pNode->left->right)
            {

            }
       }
       else if(-2 == iGrandBalnceFactor)
       {
       }
       else
       {
            assert(false);
       }
    }
    int     Insert(Node* pTree,Node* pNode)
    {
           if(Find(pNode->data))
           {
                //uniq inserting
                return -1;
           }
           Node* pParent = RawInsert(pTree,pNode);
           assert(pParent != NULL);
           Node * pGrand = pParent->parent;
           if(NULL == pGrand)
           {
                return 0;
           }
           int iGrandBalnceFactor = GetBalanceFactor();
           //case : -1, 2 , 1: 2
           //       -1,-2 , 1,-2
           if(2 == iGrandBalnceFactor)
           {
                if(pNode == pParent->left)
                {
                    //1,2 -> LLRotate
                    LLRotate(pGrand,pNode);
                }
                else
                {
                    //-1,2
                    LRRotate(pGrand,pNode);
                }
           }
           else if(-2 == iGrandBalnceFactor)
           {
                if(pNode == pParent->left)
                {
                    //1,-2
                    RLRotate(pGrand,pNode);
                }
                else
                {
                    //-1,-2
                    RRRotate(pGrand,pNode);
                }
           }
           else if(iGrandBalnceFactor > -2 &&
                   iGrandBalnceFactor < 2)
           {
                //also balance
                return 0;
           }
           else
           {
                //exception 
                //non balance , but insert one node
                assert(false);
           }
           return -1;                     
    }
    
    int     Destroy(Node* pTree)
    {

    }
    Node*   RawDelete(Node* pNode)
    {
        Node* pReplaceNode = NULL;        
        if(NULL == pNode->left && NULL == pNode->right)
        {
            //leaf ndoe
            //nothing todo
            //pReplaceNode = NULL;
        }
        else if(pNode->left && pNode->right)
        {
            // replace with its next node
            pReplaceNode = pNode->right;
            while(!pReplaceNode->left)
            {
                pReplaceNode = pReplaceNode->left;
            }

            //null or right
            pReplaceNode->parent->left = pReplaceNode->right;
            if(pReplaceNode->right)
            {
                pReplaceNode->right->parent = pReplaceNode->parent;
            }
        }
        else if(pNode->left)
        {
            //            
            pReplaceNode = pNode->left;            
        }
        else if(pNode->right)
        {
            pReplaceNode = pNode->right;
        }
        else
        {
            //no this path 
            assert(false);
            return NULL;
        }
        //modify the replace node relationship
        if(pReplaceNode )
        {
            pReplaceNode->parent = pNode->parent;    
        }        
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
            //
        }
        else
        {
            assert(pNode == root);
            root = pReplaceNode;
        }
        Destroy(pNode);
        return pReplaceNode;
    }
    int     Delete(Node* pNode)
    {
        //
        //after raw delete , rebalance it

    }
public:
    AVLTree():root(NULL);    
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




