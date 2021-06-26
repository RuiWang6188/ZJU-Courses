#include<iostream>
#include<time.h>
#include"SkipList.h"

using namespace std;

const int MAXLVL = 50;
const double P = 0.5;

SkipList::SkipList(int MaxLevel)   //constructor
{
    header = new SkipListNode(-1, MaxLevel);    //value intialize to -1
    this->MaxLevel = MaxLevel;
    level = 0;

};

// create random level for the SkipListNode 
int SkipList::randomLevel()
{
    double r = (double)rand() / RAND_MAX;
    int resLevel = 0;
    while (r < P && resLevel < MaxLevel)
    {
        r = (double)rand() / RAND_MAX;
        resLevel++;
    }
    return resLevel;
};

// create new SkipListNode 
SkipListNode* SkipList::createNode(int key, int level)
{
    SkipListNode* n = new SkipListNode(key, level);
    return n;
};

// Insert given key in skip list 
void SkipList::Insert(int value)
{
    SkipListNode* current = header;

    // create predecessors array and initialize it 
    SkipListNode* predecessors[MAXLVL + 1];
    memset(predecessors, 0, sizeof(SkipListNode*) * (MAXLVL + 1));

    /*    start from highest level of skip list
        move the current pointer forward while key
        is greater than key of node next to current
        Otherwise inserted current in predecessors and
        move one level down and continue search
    */
    for (int i = level; i >= 0; i--)
    {
        while (current->next_nodes[i] && current->next_nodes[i]->value < value) {
            current = current->next_nodes[i];
        }
        predecessors[i] = current;
    }

    /* reached level 0 and forward pointer to
       right, which is desired position to
       Insert key.
    */
    current = current->next_nodes[0];

    // value already existed in the skip list
    if (current && current->value == value) {
        cout << "The value already exist! Insert failed." << endl;
        return;
    }

    /* if current is NULL that means we have reached
       to end of the level or current's key is not equal
       to key to Insert that means we have to Insert
       SkipListNode between predecessors[0] and current SkipListNode */

    if (current == NULL || current->value != value)
    {
        // Generate a random level for SkipListNode 
        int rlevel = randomLevel();

        /* If random level is greater than list's current
           level (SkipListNode with highest level inserted in
           list so far), initialize predecessors value with pointer
           to header for further use */
        if (rlevel > level)
        {
            for (int i = level + 1; i < rlevel + 1; i++)
                predecessors[i] = header;

            level = rlevel;
        }

        // create new SkipListNode with random level generated 
        SkipListNode* n = createNode(value, rlevel);

        // Insert SkipListNode by rearranging pointers  
        for (int i = 0; i <= rlevel; i++)
        {
            n->next_nodes[i] = predecessors[i]->next_nodes[i];
            predecessors[i]->next_nodes[i] = n;
        }
        cout << "Inserted Succeed!" << endl;
    }
};

// Delete element from skip list 
void SkipList::Delete(int key)
{
    SkipListNode* current = header;
    SkipListNode* predecessors[MAXLVL + 1];
    memset(predecessors, 0, sizeof(SkipListNode*) * (MAXLVL + 1));

    /*    start from highest level of skip list
        move the current pointer forward while key
        is greater than key of SkipListNode next to current
        Otherwise inserted current in predecessors and
        move one level down and continue search
    */
    for (int i = level; i >= 0; i--)
    {
        while (current->next_nodes[i] && current->next_nodes[i]->value < key)
            current = current->next_nodes[i];
        predecessors[i] = current;
    }

    current = current->next_nodes[0];
    //if the value isn't exist, exit
    if (current && current->value != key) {
        cout << "The value isn't exist in the Skip List! Delete Fail!" << endl;
        return;
    }


    // If current SkipListNode is target SkipListNode 
    if (current != NULL && current->value == key)
    {
        /* start from lowest level and rearrange
           pointers just like we do in singly linked list
           to remove target SkipListNode */
        for (int i = 0; i <= level; i++)
        {
            /* If at level i, next SkipListNode is not target
               SkipListNode, break the loop, no need to move
              further level */
            if (predecessors[i]->next_nodes[i] != current)
                break;

            predecessors[i]->next_nodes[i] = current->next_nodes[i];
        }

        // Remove levels having no elements  
        while (level > 0 && header->next_nodes[level] == 0)
            level--;
        cout << "Delete Succeed" << endl;
    }
};

// Search for element in skip list 
void SkipList::Find(int key)
{
    SkipListNode* current = header;

    /*    start from highest level of skip list
        move the current pointer forward while key
        is greater than key of SkipListNode next to current
        Otherwise inserted current in predecessors and
        move one level down and continue search
    */
    for (int i = level; i >= 0; i--)
    {
        while (current->next_nodes[i] && current->next_nodes[i]->value < key)
            current = current->next_nodes[i];

    }

    /* reached level 0 and advance pointer to
       right, which is possibly our desired SkipListNode*/
    current = current->next_nodes[0];

    // If current SkipListNode have key equal to 
    // search key, we have found our target SkipListNode 
    if (current && current->value == key)
        cout << "Found key: " << key << "\n";
};

// Display skip list level wise 
void SkipList::Print()
{
    cout << "\n*****Skip List*****" << "\n";
    for (int i = 0; i <= level; i++)
    {
        SkipListNode* SkipListNode = header->next_nodes[i];
        cout << "Level " << i << ": ";
        while (SkipListNode != NULL)
        {
            cout << SkipListNode->value << " ";
            SkipListNode = SkipListNode->next_nodes[i];
        }
        cout << "\n";
    }
};