#include<iostream>
#include"SkipListNode.h"

SkipListNode::SkipListNode(int key, int level)
{
    next_nodes = new SkipListNode * [level + 1];
    memset(next_nodes, 0, sizeof(SkipListNode*) * (level + 1)); //initialize next_nodes array with 0
    this->value = value;
};