//
//  GroupList.h
//  Lab4
//
//  Created by Tarek Abdelrahman on 2020-10-25.
//  Copyright © 2020 Tarek Abdelrahman.
//
//  Permission is hereby granted to use this code in ECE244 at
//  the University of Toronto. It is prohibited to distribute
//  this code, either publicly or to third parties.

// ***********  ECE244 Student: DO NOT MODIFY THIS FILE  ***********

#ifndef GroupList_h
#define GroupList_h

#include "GroupNode.h"

// This class represents a linked list of shape groups
// It has a single member, head, which is a pointer to
// the first node in the list

class GroupList {
private:
    GroupNode* head;                  // Pointer to the first GroupNode of the list
public:
    GroupList();                      // sets head to nullptr
    ~GroupList();                     // deletes all list nodes, including all the ShapeList
                                      // attached to each GroupNode
    
    GroupNode* getHead() const;       // returns the head pointer
    void setHead(GroupNode* ptr);     // sets the head pointer to ptr
    void insert(GroupNode* s);        // inserts the node pointed to by s at the end of the list
    GroupNode* remove(string name);   // removes the group with the name "name" from the list
                                      // and returns a pointer to the removed GroupNode
                                      // returns nullptr is name is not found on list
    void print() const;               // prints the list, one GroupNode at a time
};

#endif /* GroupList_h */
