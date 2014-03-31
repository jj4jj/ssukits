
#pragma once


struct Pos2D
{
    int x;
    int y;    
    Pos2D(int _x,int _y):x(_x),y(_y){}
    void Reset(int _x,int _y)
    {
        x = _x;
        y = _y;
    }
};

class AstarNode;
class AstarNode
{
    void* pData;
    int   iVisit;
    int   g;
    AstarNode * parent;    
};


class AStar2D
{
public:
    int    Init(int Width,int iHeight);
    //writable
    AstarNode &   GetNode(Pos2D pos);
    int    ReCalcPath(Pos2D src,Pos2D dst)
    {
        vecPath.clear();
        openList.clear();
        closeList.clear();
        vecSrc.x = src.x;
        vecSrc.y = src.y;

        vecDst.x = dst.x;
        vecDst.y = dst.y;

        /////extend all src adj 
        if(ExtendNode(src) < 0)
        {
            return -1;
        }
        iVstMagic++;
        //
        assert(iVstMagic != 0);
        SetPosVisited(src);
        InsertIntoCloseList(src);
        return 0;
    }
    void    SetPosVisited(const Pos2D & pos)
    {
        if(IsPosInValid(pos)
        {
            return;
        }
        AstarNode& node = GetNode(pos);
        node.iVisit = iVstMagic;        
    }
    void    InsertIntoOpenList(const Pos2D& pos)
    {
        SetPosVisited(pos);
        //push one node
        openList.push_back(pos);        
    }
    
    void    InsertIntoCloseList(const Pos2D& pos)
    {
        closeList.push_back(pos);
    }
        
    bool    IsPosInValid(int x,int y)
    {
        return (x < 0 || x >= iMapWidth ||
           y < 0 || y >= iMapHeight);
    }
    int     GetNodeNeighbors(Pos2D pos,vector<Pos2D> &ngbs)
    {
        //up right down left
        int iDeltaX[] = {0,1,0,-1};
        int iDeltaY[] = {1,0,-1,0};
        for(int i = 0 ;i < 4; ++i)
        {
            Pos2D npos = (pos.x+iDeltaX[i],pos.y+iDeltaY[i]);
            if(IsPosInValid(npos.x,npos.y))
            {
                continue;
            }
            ngbs.push_back(npos);
        }
        return 0;
    }
    //return 0 is ok
    //else error
    int     PopMinCostNodePos(Pos2D & pos)
    {
        /////
        if(openList.empty())
        {
            return -1;
        }
        //priority queue todo
        pos = openList.back();
        openList.pop_back();

        //////////////////////
        return 0;
    }
    bool   IsPosVisited(const Pos2D& pos)
    {
        if(IsPosInValid(pos))
        {
            return true;
        }
        return (GetNode(pos).iVisit == iVstMagic);
    }
    void   SetPFNIsPathNode(PFNIsPathNode _pfnIsPathNode)
    {
        pfnIsPathNode = _pfnIsPathNode;
    }
    int    ExtendNode(Pos2D& pos)
    {
        vector < Pos2D > ngbs;        
        GetNodeNeighbors(pos,ngbs);
        if(ngbs.empty())
        {
            return -1;
        }

        AstarNode & snode = GetNode(pos);
        
        for(int i = 0; i < ngbs.size(); ++i)
        {
            if(IsPosVisited(ngbs[i]))
            {
                continue;
            }
            //////////////////////////////////            
            AstarNode & node = GetNode(ngbs[i]);
            SetPosVisited(ngbs[i]);
            if(!pfnIsPathNode(node))
            {
                continue;
            }
            
            node.parent = &snode;
            node.g = snode.g+1;                
            ///////////////////////////            
            if(ngbs[i] == posDst)
            {
                return 0;
            }
            InsertIntoOpenList(ngbs[i]);
        }
        return 1;        
    }
    //when return 0 , path is cacled ok.
    //return > 1, need again
    //return < 0, error
    int    CalcStepPath(int iStepNum)
    {        
        Pos2D pos;
        int iStep = 0;
        do
        {
            if(PopMinCostNodePos(pos))
            {            
                return -1;
            }
            InsertIntoCloseList(pos);            
            //return 0 is ok
            //return < 0 is fail
            //return > 0 need again
            int ret = ExtendNode(pos);
            if(0 == ret)
            {
                return 0;
            }
            else if( ret < 0 )
            {
                return -1;
            }
            iStep++;
        }while(iStep < iStepNum );
        //need again
        return 1;
    }
    const vector<Pos2D> & GetPath()
    {
        AstarNode * node = &GetNode(posDst);
        if(node->iVisit != iVstMagic)
        {
            vecPath.clear();
            ////////////////
            do
            {
                vecPath.push_back(GetNodePos(*node));
                node = node->parent
            }
            while(node != NULL);           
        }
        return vecPath;
    }
    void   Destroy();

public:
    typedef (PFNIsPathNode *)(AstarNode& node);
private:
    int    iMapWidth;
    int    iMapHeight;
    Pos2D  posSrc;
    Pos2D  posDst;
    AstarNode*      p2DMeshData;
    vector<Pos2D>   vecPath;
    vector<Pos2D>   openList;
    vector<Pos2D>   closeList;
    int     iVstMagic;
    PFNIsPathNode    pfnIsPathNode;
};




