//
//  GroupNode.h
//  Lab4
//
//  Created by Tarek Abdelrahman on 2020-10-25.
//  Copyright © 2020 Tarek Abdelrahman.
//
//  Permission is hereby granted to use this code in ECE244 at
//  the University of Toronto. It is prohibited to distribute
//  this code, either publicly or to third parties.

// ***********  ECE244 Student: DO NOT MODIFY THIS FILE  ***********

#ifndef GroupNode_h
#define GroupNode_h

#include "ShapeList.h"

// This class implements a node in the GroupList.
// Each GroupNode has the name of the group,
// a pointer to a ShapeList and a pointer to the
// next node in the list

class GroupNode {
private:
    string name;                           // name of the group
    ShapeList* myShapeList;                // pointer to the list of shapes in group
    GroupNode* next;                      // pointer to next node in the list
public:
    GroupNode(string n);                   // sets group name to n and makes myShapeList
                                           // point to a new ShapeList; sets next to nullptr
    ~GroupNode();                          // deletes the myShapeList list
    string getName() const;                // returns group name
    ShapeList* getShapeList() const;       // returns myShapeList
    void setShapeList(ShapeList* ptr);     // sets myShapeList to ptr
    GroupNode* getNext() const;            // returns next
    void setNext(GroupNode* ptr);          // sets next to ptr
    void print() const;                    // prints the GroupNode
};

#endif /* GroupNode_h */
