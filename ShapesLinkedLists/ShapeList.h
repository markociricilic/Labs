//
//  ShapeList.h
//  Lab4
//
//  Created by Tarek Abdelrahman on 2020-10-25.
//  Copyright © 2020 Tarek Abdelrahman.
//
//  Permission is hereby granted to use this code in ECE244 at
//  the University of Toronto. It is prohibited to distribute
//  this code, either publicly or to third parties.

// ***********  ECE244 Student: DO NOT MODIFY THIS FILE  ***********

#ifndef ShapeList_h
#define ShapeList_h

#include "Shape.h"
#include "ShapeNode.h"

// This class represents a list of Shape nodes
// It has a single member, head, which points
// to the first node in the list

class ShapeList {
private:
    ShapeNode* head;
public:
    ShapeList();                        // sets head to nullptr
    ~ShapeList();                       // deletes all list nodes starting
                                        // with head plus associated Shapes
    
    ShapeNode* getHead() const;         // returns the head pointer
    void setHead(ShapeNode* ptr);       // sets the head pointer to ptr
    
    ShapeNode* find(string name) const; // finds a shape node with the specified shape name
                                        // returns a pointer to the node if found, otherwise
                                        // returns nullptr if the node is not found
    void insert(ShapeNode* s);          // inserts the node pointed to by s at the end
                                        // of the list; s should never be nullptr
    ShapeNode* remove(string name);     // removes the node with the shape called name
                                        // returns a pointer to the removed node or
                                        // nullptr is a node with name does not exist
    void print() const;                 // prints the list
};

#endif /* ShapeList_h */
