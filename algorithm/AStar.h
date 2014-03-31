
#pragma once



struct Pos2D
{
    int x;
    int y;    
    Pos2D(int _x,int _y):x(_x),y(_y){}
    void Reset(int _x = 0,int _y = 0)
    {
        x = _x;
        y = _y;
    }
    Pos2D & operator = (const Pos2D& pos)
    {
        x = pos.x;
        y = pos.y;
    }
    int ManhattonDist(const Pos2D& pos)
    {
        return sqrt((pos.x-x)*(pos.x-x) + (pos.y-y)*(pos.y-y));
    }
    
    int EulerDist(const Pos2D& pos)
    {
        return abs(pos.y - y) + abs(pos.x - x);
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
    ~AStar2D();
    int    Init(int iWidth,int iHeight);
    //writable
    AstarNode *   GetNode(Pos2D pos);
    int    ReCalcPath(Pos2D src,Pos2D dst);
    void    SetPosVisited(const Pos2D & pos);
    void    InsertIntoOpenList(const Pos2D& pos);    
    void    InsertIntoCloseList(const Pos2D& pos);       
    bool    IsPosInValid(int x,int y);
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




