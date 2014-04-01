

#include "AStar.h"
#include "base/Log.h"
#include "base/CommonMacro.h"

AStar2D::~AStar2D()
{
    Destroy();
}
int    AStar2D::Init(int iWidth,int iHeight)
{
    iVstMagic = 0;
    assert(iWidth*iHeight > 0);
    p2DMeshData = new AstarNode[iWidth*iHeight];
    if(NULL == p2DMeshData)
    {
        return -1;
    }
    iMapWidth = iWidth;
    iMapHeight = iHeight;
    vecPath.clear();
    openList.clear();
    closeList.clear();
    pfnIsPathNode = NULL;
    posSrc.Reset();
    posDst.Reset(0);
    return 0;
}
//writable
AstarNode * AStar2D::GetNode(Pos2D pos)
{
    if(IsPosInValid(pos))
    {
        return NULL;
    }
    return p2DMeshData + pos.x*iMapWidth+pos.y;
}
int    AStar2D::ReCalcPath(Pos2D src,Pos2D dst)
{
    vecPath.clear();
    openList.clear();
    closeList.clear();
    posSrc = src;
    posDst = dst;

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
void    AStar2D::SetPosVisited(const Pos2D & pos)
{
    if(IsPosInValid(pos))
    {
        return;
    }
    AstarNode* node = GetNode(pos);
    if(node)
    {
        node->iVisit = iVstMagic;
    }
}
void    AStar2D::InsertIntoOpenList(const Pos2D& pos)
{
    SetPosVisited(pos);
    //push one node
    openList.push_back(pos);
}

void    AStar2D::InsertIntoCloseList(const Pos2D& pos)
{
    closeList.push_back(pos);
}

bool    AStar2D::IsPosInValid(const Pos2D & pos)
{
    return (pos.x < 0 || pos.x >= iMapWidth ||
       pos.y < 0 || pos.y >= iMapHeight);
}
int     AStar2D::GetNodeNeighbors(Pos2D pos,vector<Pos2D> &ngbs)
{
    //up right down left
    int iDeltaX[] = {0,1,0,-1};
    int iDeltaY[] = {1,0,-1,0};
    for(int i = 0 ;i < 4; ++i)
    {
        Pos2D npos(pos.x+iDeltaX[i],pos.y+iDeltaY[i]);
        if(IsPosInValid(npos))
        {
            continue;
        }
        ngbs.push_back(npos);
    }
    return 0;
}
//return 0 is ok
//else error
int     AStar2D::PopMinCostNodePos(Pos2D & pos)
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
bool   AStar2D::IsPosVisited(const Pos2D& pos)
{
    AstarNode* node = GetNode(pos);
    if(NULL == node)
    {
        return true;
    }
    return (node->iVisit == iVstMagic);
}
void   AStar2D::SetPFNIsPathNode(PFNIsPathNode _pfnIsPathNode)
{
    pfnIsPathNode = _pfnIsPathNode;
}
int    AStar2D::ExtendNode(Pos2D& pos)
{
    vector < Pos2D > ngbs;
    GetNodeNeighbors(pos,ngbs);
    if(ngbs.empty())
    {
        return -1;
    }

    AstarNode * snode = GetNode(pos);
    if(NULL == snode)
    {
        return -1;
    }

    for(int i = 0; i < ngbs.size(); ++i)
    {
        if(IsPosVisited(ngbs[i]))
        {
            continue;
        }
        //////////////////////////////////
        AstarNode * node = GetNode(ngbs[i]);
        assert(node != NULL);
        SetPosVisited(ngbs[i]);
        if(!pfnIsPathNode(*node))
        {
            continue;
        }

        node->parent = snode;
        node->g = snode->g+1;
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
int    AStar2D::CalcStepPath(int iStepNum)
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
Pos2D AStar2D::GetInnerNodePos(const AstarNode* node)
{
    int iOffset = node - p2DMeshData;
    assert(iOffset >= 0 && iOffset < iMapWidth*iMapHeight);
    return Pos2D(iOffset/iMapWidth,iOffset%iMapWidth);
}

const vector<Pos2D> & AStar2D::GetPath( )
{
    AstarNode * node = GetNode(posDst);
    if(node->iVisit != iVstMagic)
    {
        vecPath.clear();
        ////////////////
        do
        {
            vecPath.push_back(GetInnerNodePos(node));
            node = node->parent;
        }
        while(node != NULL);
    }
    return vecPath;
}
void   AStar2D::Destroy()
{
    SAFE_DELETE(p2DMeshData);
    vecPath.clear();
    iVstMagic = 0;
}

