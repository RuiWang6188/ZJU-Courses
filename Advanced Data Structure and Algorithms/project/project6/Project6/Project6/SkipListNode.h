#pragma once
#ifndef _SKIP_LIST_NODE
#define _SKIP_LIST_NODE


class SkipListNode
{
public:
    int value;
    SkipListNode** next_nodes;  // Array to hold pointers to node of different level  
    SkipListNode(int value, int level);
};



#endif //_SKIP_LIST_NODE