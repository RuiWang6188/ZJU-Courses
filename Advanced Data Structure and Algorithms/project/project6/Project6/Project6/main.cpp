#include<iostream>
#include<time.h>
#include"SkipList.h"

int main()
{
    // Seed random number generator 
    srand((unsigned)time(0));

    // create SkipList object with MAXLVL
    SkipList lst(3);

    lst.Insert(3);
    lst.Insert(6);
    lst.Insert(7);
    lst.Insert(9);
    lst.Insert(12);
    lst.Insert(19);
    lst.Insert(17);
    lst.Insert(26);
    lst.Insert(21);
    lst.Insert(25);
    lst.Print();

    //Search for node 19 
    lst.Find(19);

    //Delete node 19 
    lst.Delete(19);
    lst.Print();
}