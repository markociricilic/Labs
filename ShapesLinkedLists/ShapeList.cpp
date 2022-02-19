//
//  ShapeList.cpp
//  Lab4
//
//  Created by Tarek Abdelrahman on 2020-10-25.
//  Copyright © 2020 Tarek Abdelrahman.
//
//  Permission is hereby granted to use this code in ECE244 at
//  the University of Toronto. It is prohibited to distribute
//  this code, either publicly or to third parties.

//  ECE244 Student:
//  Write the implementation (and only the implementation) of the ShapeList class below

#include <iostream>
#include <string>

using namespace std;

#include "ShapeList.h"

// sets head to nullptr
ShapeList::ShapeList() {
    head = nullptr;
}       

// deletes all list nodes starting with head plus associated Shapes
ShapeList::~ShapeList() {
    ShapeNode* tptr = head;
    ShapeNode* pptr = nullptr;
    
    while (tptr != nullptr) {
        pptr = tptr->getNext();
        delete tptr;
        tptr = pptr;
    }
    
}                      

// returns the head pointer
ShapeNode* ShapeList::getHead() const {
    return head;
}        

// sets the head pointer to ptr
void ShapeList::setHead(ShapeNode* ptr) {
    head = ptr;
}     

// finds a shape node with the specified shape name returns a pointer to the node if found, otherwise returns nullptr if the node is not found
ShapeNode* ShapeList::find(string name) const {
    ShapeNode* tptr = head;
    
    if (tptr == nullptr) 
        return nullptr;
    
    while (tptr != nullptr) {
        if (tptr->getShape()->getName() == name) {
            return tptr;
        }
        tptr = tptr->getNext();
    }
    return nullptr;
}

// inserts the node pointed to by s at the end of the list; s should never be nullptr
void ShapeList::insert(ShapeNode* s) {
    if (head == nullptr) {
        head = s;
        s->setNext(nullptr);
    } else {
        ShapeNode* tptr = head;
        
        while (tptr->getNext() != nullptr) {
            tptr = tptr->getNext();
        }
        tptr->setNext(s);
        s->setNext(nullptr);
    }
} 

// removes the node with the shape called name returns a pointer to the removed node or nullptr is a node with name does not exist
ShapeNode* ShapeList::remove(string name) {
    ShapeNode* tptr = head;
    
    if (tptr == nullptr) {
        return tptr;
    }

    ShapeNode* pptr = tptr->getNext();
    
    if (tptr->getShape()->getName() == name) {
        head=pptr;
        return tptr;
    }

    while (pptr != nullptr) {
        if (pptr->getShape()->getName() == name) {
            tptr->setNext(pptr->getNext());
            return pptr;
        }
        tptr = tptr->getNext();
        pptr = pptr->getNext();
    }
    return pptr;
}

// prints the list
void ShapeList::print() const {
    ShapeNode* tptr = head;
    
    while (tptr != nullptr) {
        tptr->print();
        tptr = tptr->getNext();
    }
}       