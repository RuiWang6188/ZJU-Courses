#pragma once
#ifndef _SKIP_LIST
#define _SKIP_LIST

#include"SkipListNode.h"

class SkipList
{
    int MaxLevel;       // Maximum level of the skip list 
    int level;          // current level of the skip list
    SkipListNode* header;
public:
    SkipList(int MaxLevel);
    int randomLevel();  // function to generate random level
    SkipListNode* createNode(int value, int level);
    void Find(int value);   //find elemenet in skip list
    void Insert(int value); //insert element into skip list
    void Delete(int value); //delete element in skip list
    void Print();       //print the skip list
};


#endif //_SKIP_LIST