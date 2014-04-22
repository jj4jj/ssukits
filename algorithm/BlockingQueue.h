
#pragma once


struct BlockingQueueNode;
struct BlockingQueueNode
{
    void*                data;    
    BlockingQueueNode   *prev,*next;
};


class BlockingQueue
{
public:
    int     Push(void*  pData);
    int     Pop(void** ppData);
    int     Peek(void** ppData);
    int     GetLength();
private:
    BlockingQueueNode*  pHead;
    BlockingQueueNode*  pTail;
};
















