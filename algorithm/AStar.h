
#pragma once

#include "base/stdinc.h"

struct Pos2D
{
    int x;
    int y;
    Pos2D(int _x = 0,int _y = 0):x(_x),y(_y){}
    void Reset(int _x = 0,int _y = 0)
    {
        x = _x;
        y = _y;
    }
    Pos2D & operator = (const Pos2D& pos)
    {
        if(&pos == this )
        {
            return *this;
        }
        x = pos.x;
        y = pos.y;
        return *this;
    }
    int ManhattonDist(const Pos2D& pos)
    {
        return sqrt((pos.x-x)*(pos.x-x) + (pos.y-y)*(pos.y-y));
    }

    int EulerDist(const Pos2D& pos)
    {
        return abs(pos.y - y) + abs(pos.x - x);
    }
    bool operator == (const Pos2D& pos)
    {
        return (x == pos.x && y == pos.y);
    }
};

struct AstarNode;
struct AstarNode
{
    void* pData;
    int   iVisit;
    int   g;
    AstarNode * parent;
};

//the map position is as follow
//and when extending one node , the extend order is
//  up -> right -> down -> left
/*************************************

    0  -1---2-   ...     iMapWidth-1
    |                       |
    1  -|---|-   ...    ----|
    |                       |
    2  -|---|-   ...    ----|
    |                       |
    .   .   .     .         |
    .   .   .     .         |
    .   .   .     .         |
    |                       |
    iMapHeight-1  ...   ----.

***************************************/


class AStar2D
{
public:
    typedef bool (* PFNIsPathNode)(AstarNode& node);
public:
    ~AStar2D();
    int    Init(int iWidth,int iHeight);
    //writable
    AstarNode *   GetNode(Pos2D pos);
    int    ReCalcPath(Pos2D src,Pos2D dst);
    void    SetPosVisited(const Pos2D & pos);
    void    InsertIntoOpenList(const Pos2D& pos);
    void    InsertIntoCloseList(const Pos2D& pos);
    bool    IsPosInValid(const Pos2D & pos);
    int     GetNodeNeighbors(Pos2D pos,vector<Pos2D> &ngbs);
     //return 0 is ok
    //else error
    int     PopMinCostNodePos(Pos2D & pos);
    bool   IsPosVisited(const Pos2D& pos);
    void   SetPFNIsPathNode(PFNIsPathNode _pfnIsPathNode);
    int    ExtendNode(Pos2D& pos);
     //when return 0 , path is cacled ok.
    //return > 1, need again
    //return < 0, error
    int    CalcStepPath(int iStepNum);
    Pos2D GetInnerNodePos(const AstarNode* node);
    const vector<Pos2D> & GetPath();
    void   Destroy();
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




