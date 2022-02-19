//
//  GroupList.cpp
//  Lab4
//
//  Created by Tarek Abdelrahman on 2020-10-25.
//  Copyright © 2020 Tarek Abdelrahman.
//
//  Permission is hereby granted to use this code in ECE244 at
//  the University of Toronto. It is prohibited to distribute
//  this code, either publicly or to third parties.

//  ECE244 Student:
//  Write the implementation (and only the implementation) of the GroupList class below

#include <iostream>
#include <string>

using namespace std;

#include "ShapeList.h"
#include "GroupList.h"             

// sets head to nullptr
GroupList::GroupList() {
    head = nullptr;
}   

// deletes all list nodes, including all the ShapeList attached to each GroupNode
GroupList::~GroupList() {
    GroupNode* tptr = head;
    GroupNode* pptr = nullptr;
    
    while (tptr != nullptr) {
        pptr = tptr->getNext();
        delete tptr;
        tptr = pptr;
    }

}                   

// returns the head pointer
GroupNode* GroupList::getHead() const {
    return head;
}     

// sets the head pointer to ptr
void GroupList::setHead(GroupNode* ptr) {
    head = ptr;
}    

// inserts the node pointed to by s at the end of the list
void GroupList::insert(GroupNode* s) {
    if (head == nullptr) {
        head = s;
        s->setNext(nullptr);
    } else {
        GroupNode* tptr = head;

        while (tptr->getNext() != nullptr) {
            tptr = tptr->getNext();
        }
        tptr->setNext(s);
        s->setNext(nullptr);
    }
}

// removes the group with the name "name" from the list and returns a pointer to the removed GroupNode or returns nullptr is name is not found on list
GroupNode* GroupList::remove(string name) {
    GroupNode* tptr = head;
    
    if (tptr == nullptr) {
        return tptr;
    }

    GroupNode* pptr = tptr->getNext();
    
    if (tptr->getName() == name) {
        head = pptr;
        return tptr;
    }
                
    while (pptr != nullptr) {
        if (pptr->getName() == name) {
            tptr->setNext(pptr->getNext());
            return pptr;
        }
        tptr = tptr->getNext();
        pptr = pptr->getNext();
    }
    return pptr;
}

// prints the list, one GroupNode at a time
void GroupList::print() const {
    GroupNode* tptr = head;
    
    while (tptr != nullptr) {
        tptr->print();
        tptr = tptr->getNext();
    }
}              