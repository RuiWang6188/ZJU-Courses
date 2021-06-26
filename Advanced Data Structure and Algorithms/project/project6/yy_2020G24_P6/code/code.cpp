// C++ code for searching and deleting element in skip list 

#include <iostream>
#include<time.h>
#include <random>
using namespace std;

const float P = 0.5;
const int MaxLevel = 32;

clock_t start_time;
clock_t end_time;

// Class to implement node 
class Node
{
public:
    int value;

    // Array to hold pointers to node of different level  
    Node** next_nodes;
    Node(int value, int level);
};

Node::Node(int value, int level)
{
    this->value = value;

    // Allocate memory to next_nodes  
    next_nodes = new Node * [level + 1];

    // Fill next_nodes array with 0(NULL) 
    memset(next_nodes, 0, sizeof(Node*) * (level + 1));
};

// Class for Skip list 
class SkipList
{
     
    int MAXLEVEL;    // Maximum level allowed for this skip list
    int level;      // current level of skip list
    Node* H;        // pointer to header node 
public:
    SkipList(int MAXLEVEL); // constructor
    int randomLevel();
    Node* createNode(int value, int level);
    void Find(int value);   // Find element from skip list 
    void Insert(int value); // Insert element from skip list 
    void Delete(int value); // Delete element from skip list 
    void printList(void);   //print the skip list
};

SkipList::SkipList(int MAXLEVEL)
{
    level = 0;   
    this->MAXLEVEL = MAXLEVEL;
    H = new Node(-1, MAXLEVEL); // create header node and initialize value to -1 
};

// create random level for node 
int SkipList::randomLevel()
{
    int level = 0;
    float r = (float)rand() / RAND_MAX;

    for (; level < MAXLEVEL && r < P; level++) {
        r = (float)rand() / RAND_MAX;
    }

    //while (r < P && level < MAXLEVEL)
    //{
    //    level++;
    //    r = (float)rand() / RAND_MAX;
    //}
    return level;
};

// create new node 
Node* SkipList::createNode(int value, int level)
{
    Node* N = new Node(value, level);
    return N;
};

// Insert given value in skip list 
void SkipList::Insert(int value)
{
    Node* current = H;

    Node* predecessors[MaxLevel + 1];  //store the successors 
    memset(predecessors, 0, sizeof(Node*) * (MAXLEVEL + 1));

    
    for (int i = level; i >= 0; i--)    //Start from the top level to search down to the deepest level
    {
        while (current->next_nodes[i] && current->next_nodes[i]->value < value)
            current = current->next_nodes[i];
        predecessors[i] = current;
    }

    current = current->next_nodes[0];       //find the position

    if (current && current->value == value) {    // value already in the list
        cout << "INSERT FAILED: Insert Duplicate Values!" << endl;
        return;
    }

    if (!current || current->value != value)   // not in the list, we can perform insert
    {
        
        int rlevel = randomLevel(); // Generate a random level for node 

        if (rlevel > level)
        {
            for (int i = level + 1; i < rlevel + 1; i++)
                predecessors[i] = H;

            level = rlevel; // predecessors the list current level 
        }

      
        Node* N = createNode(value, rlevel);        // create the node

        
        for (int i = 0; i <= rlevel; i++)   // insert node by rearranging pointers  
        {
            N->next_nodes[i] = predecessors[i]->next_nodes[i];
            predecessors[i]->next_nodes[i] = N;
        }
        //cout << "INSERT SUCCESS:" << value << endl;
    }
};


void SkipList::Delete(int value)
{
    Node* current = H;

    Node* predecessors[MaxLevel + 1];
    memset(predecessors, 0, sizeof(Node*) * (MAXLEVEL + 1));

    for (int i = level; i >= 0; i--)    //Similar the insertion, find the value from the top to down level
    {
        while (current->next_nodes[i] && current->next_nodes[i]->value < value)
            current = current->next_nodes[i];
        predecessors[i] = current;
    }

    current = current->next_nodes[0];   // find the position

    if (current && current->value != value) {
        cout << "DELETE FAILED: Value Not in the List!" << endl;
        return;
    }

    if (current and current->value == value)
    {
        for (int i = 0; i <= level; i++)
        {
            if (predecessors[i]->next_nodes[i] != current)  //if not exist, no need to continue
                break;

            predecessors[i]->next_nodes[i] = current->next_nodes[i];
        }

        
        while (level > 0 && H->next_nodes[level] == 0)
            level--;
        //cout << "DELETE SUCCESS:" << value << endl;
    }
};

void SkipList::Find(int value)
{
    Node* current = H;

    for (int i = level; i >= 0; i--)
    {
        while (current->next_nodes[i] && current->next_nodes[i]->value < value)
            current = current->next_nodes[i];
    }

    current = current->next_nodes[0];       // find the postion


    if (current and current->value != value)
        cout << "SEARCH FAILED:" << value << endl;
    else
        cout << "SEARCH SUCCESS:" << value << endl;
};

// Display skip list level wise 
void SkipList::printList()
{
    Node* currentNode;
    cout << "Skip List:" << endl;
    for (int i = level; i >= 0; i--){
        currentNode = H->next_nodes[i];
        cout << "Level " << i << ": ";
        while (currentNode)
        {
            cout << currentNode->value << " ";
            currentNode = currentNode->next_nodes[i];
        }
        cout << endl;
    }
};

int main()
{
       srand((unsigned)time(0));

    int N;
    cout << "Input the N:";
    cin >>  N;
 
    std::random_device rd; // obtain a random number from hardware
    std::mt19937 eng(rd()); // seed the generator
    std::uniform_int_distribution<> distr(0, N - 1); // define the range

    // create SkipList object with MAXLEVEL 
    SkipList L(MaxLevel);
/******     Insert phase   ***********/
    start_time = clock();
    for (int i = 0; i < N; i++) {   //insert
        L.Insert(i);
    }
    end_time = clock();

    L.printList();
    cout << "Insert Time: " << (float)(end_time - start_time) / CLOCKS_PER_SEC << endl;


 /******   Search phase     ********/
    //start_time = clock();
    //for (int i = 0; i < N; i++) {   
    //    L.Find(distr(eng));
    //}
    //end_time = clock();
    //cout << "Search Time: " << (float)(end_time - start_time) / N * CLOCKS_PER_SEC << endl;

 /******   Delete phase     ********/
    //start_time = clock();
    //for (int i = 0; i < N; i++) {   //insert
    //    L.Delete(i);
    //}
    //end_time = clock();

    //L.printList();
    //cout << "Delete Time: " << (float)(end_time - start_time) / CLOCKS_PER_SEC << endl;

    system("pause");
}