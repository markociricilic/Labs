//
//  parser.cpp
//  Lab4
//
//  Created by Tarek Abdelrahman on 2020-10-25.
//  Copyright © 2020 Tarek Abdelrahman.
//
//  Permission is hereby granted to use this code in ECE244 at
//  the University of Toronto. It is prohibited to distribute
//  this code, either publicly or to third parties.


#include <iostream>
#include <sstream>
#include <string>

using namespace std;

#include "globals.h"
#include "Shape.h"
#include "ShapeNode.h"
#include "GroupNode.h"
#include "ShapeList.h"
#include "GroupList.h"

// This is a pointer to the groups list
// The list itself must be allocated
GroupList* gList;

// ECE244 Student: you may want to add the prototype of
// helper functions you write here

void shape (stringstream &line);
void group (stringstream &line);
void draw ();
void move (stringstream &line);
void deleteShapeGroup (stringstream &line);

bool invalidName (string name);
bool repeatingName (string name);
bool nonExistantName (string name);
bool repeatingGroup (string name);
bool nonExistantGroup (string name);

int main() {
    // Create the groups list
    gList = new GroupList();
    
    // Create the poo group and add it to the group list
    GroupNode* poolGroup = new GroupNode(keyWordsList[NUM_KEYWORDS-1]);
    gList->insert(poolGroup);
    
    string line;
    string command;
    
    cout << "> ";         // Prompt for input
    getline(cin, line);    // Get a line from standard input

    while ( !cin.eof () ) {
        // Put the line in a linestream for parsing
        // Making a new sstream for each line so flags etc. are cleared
        stringstream lineStream (line);
        
        // Read from string stream into the command
        // The only way this can fail is if the eof is encountered
        lineStream >> command;

        // Check for the command and act accordingly
                
        bool checkCommand = false;
        
        for (int i = 0; i < NUM_KEYWORDS; i++) {
            if (command == keyWordsList[i])
                checkCommand = true;
        }
        
        if (checkCommand == false) {
            cout << "Error: invalid command" << endl;
        }

        if (checkCommand) {
            if (command == "shape") 
                shape(lineStream);
            else if (command == "group") 
                group(lineStream);
            else if (command == "draw") 
                draw();
            else if (command == "move") 
                move(lineStream);
            else if (command == "delete") 
                deleteShapeGroup(lineStream);
        }
        
        lineStream.clear();
        
        // Once the command has been processed, prompt for the
        // next command
        cout << "> ";          // Prompt for input
        getline(cin, line);
        command = "";
    }  // End input loop until EOF.
    
    return 0;
}

void shape (stringstream &line) {
    
    string name, type;
    int x_location, y_location, x_size, y_size;
        
    line >> name;
    
    if (!(line.fail() && line.eof())) {
        if (invalidName(name)) {
            return;
        } else if (repeatingName(name) || repeatingGroup(name)) {
            return;
        } else {
            line >> type >> x_location >> y_location >> x_size >> y_size;
            
            Shape* shape = new Shape(name, type, x_location, y_location, x_size, y_size);
            ShapeNode* nptr = new ShapeNode();
            nptr->setShape(shape);
            
            nptr->getShape()->setName(name);
            nptr->getShape()->setType(type);
            nptr->getShape()->setXlocation(x_location);
            nptr->getShape()->setYlocation(y_location);
            nptr->getShape()->setXsize(x_size);
            nptr->getShape()->setYsize(y_size);
            nptr->getShape()->draw();

            GroupNode* insertShape = gList->getHead();
            insertShape->getShapeList()->insert(nptr);
        }
    }
}

void group (stringstream &line) {
    
    string name;
    line >> name;
    
    if (!(line.fail() && line.eof())) {
        if (invalidName(name)) {
            return;
        } else if (repeatingName(name) || repeatingGroup(name)) {
            return;
        } else {
            GroupNode* gptr = new GroupNode(name);
            gList->insert(gptr);
            cout << name << ": group" << endl;
        }
    }
}

void draw () {
    cout << "drawing: " << endl;
    gList->print();
}

void move (stringstream &line) {
    
    string nameShape;
    string nameGroup;
    
    line >> nameShape;
    line >> nameGroup;
    
    if (!(line.fail() && line.eof())) {
        if (invalidName(nameShape) || invalidName(nameGroup)) {
            return;
        } else {
            if (nonExistantName(nameShape)) {
                cout << "error: shape " << nameShape << " not found" << endl;
                return;
            } else if (nonExistantGroup(nameGroup)) {
                cout << "error: group " << nameGroup << " not found" << endl;
                return;
            }

            ShapeNode* locateS = nullptr;
            GroupNode* ptr = gList->getHead();

            while (ptr != nullptr) {
                if (ptr->getShapeList()->find(nameShape) != nullptr) {
                    locateS = ptr->getShapeList()->remove(nameShape);
                }
                ptr = ptr->getNext();
            }
            
            if (locateS == nullptr) {
                return; 
            }
            
            locateS->setNext(nullptr);
            GroupNode* locateG = gList->getHead();
            
            while (locateG != nullptr) {
                if (locateG->getName() == nameGroup) {
                    locateG->getShapeList()->insert(locateS);
                    cout << "moved " << nameShape << " to " << nameGroup << endl;
                    return;
                }
                locateG = locateG->getNext();
            } 
        }
    }
}

void deleteShapeGroup (stringstream &line) {
    
    string name;
    
    line >> name;
    
    if (!(line.fail() && line.eof())) {
        if (invalidName(name)) {
            return;
        } else {
            if (nonExistantName(name) && nonExistantGroup(name)) {
                cout << "error: shape " << name << " not found" << endl;
                return;
            }
            
            // delete shape
            ShapeNode* removeS = gList->getHead()->getShapeList()->find(name);

            if (removeS != nullptr) {
                removeS = gList->getHead()->getShapeList()->remove(name);
                delete removeS;
                cout << name << ": deleted" << endl;
                return;
            } else {
                GroupNode* removeSG = gList->getHead()->getNext();

                while (removeSG != nullptr) {
                    if (removeSG->getShapeList()->find(name) != nullptr) {
                        removeS = removeSG->getShapeList()->remove(name);
                        delete removeS;
                        cout << name << ": deleted" << endl;
                        return;
                    }
                    removeSG = removeSG->getNext();
                }
            }

            // delete group
            GroupNode* removeG = gList->remove(name);
            GroupNode* returnPool = gList->getHead();

            while (removeG->getShapeList()->getHead() != nullptr) {
                string s = removeG->getShapeList()->getHead()->getShape()->getName();
                ShapeNode* tsptr = removeG->getShapeList()->remove(s);
                returnPool->getShapeList()->insert(tsptr);
            }

            removeG->getShapeList()->setHead(nullptr);
            delete removeG;
            cout << name << ": deleted" << endl;
            return;
        }
    }
}

bool invalidName (string name) {

    if (name == "shape" || name == "group" || name == "move" || name == "delete" || name == "draw" || name == "pool" || name == "ellipse" || name == "rectangle" || name == "triangle") {
        cout << "error: invalid name" << endl;
        return true;
    }
    return false;
}

bool repeatingName (string name) {
    ShapeNode* existingName = gList->getHead()->getShapeList()->getHead();
    
    while (existingName != nullptr) {
        if (existingName->getShape()->getName() == name) {
            cout << "error: name " << name << " exists" << endl;
            return true;
        }
        existingName = existingName->getNext();
    }
    
    GroupNode* locateSG = gList->getHead()->getNext();

    while (locateSG != nullptr) {
        if (locateSG->getShapeList()->find(name)) {
            cout << "error: name " << name << " exists" << endl;
            return true;
        }
        locateSG = locateSG->getNext();
    }
    return false;
}

bool repeatingGroup (string name) {
    GroupNode* existingGroup = gList->getHead();
    
    while (existingGroup != nullptr) {
        if (existingGroup->getName() == name) {
            cout << "error: name " << name << " exists" << endl;
            return true;
        }
        existingGroup = existingGroup->getNext();
    }
    return false;
}

bool nonExistantName (string name) {
    ShapeNode* existingName = gList->getHead()->getShapeList()->getHead();

    while (existingName != nullptr) {
        if (existingName->getShape()->getName() == name) {
            return false;
        }
        existingName = existingName->getNext();
    }
    
    GroupNode* locateSG = gList->getHead()->getNext();

    while (locateSG != nullptr) {
        if (locateSG->getShapeList()->find(name)) {
            return false;
        }
        locateSG = locateSG->getNext();
    }
    return true;
}

bool nonExistantGroup (string name) {
    GroupNode* existingGroup = gList->getHead();
    
    while (existingGroup != nullptr) {
        if (existingGroup->getName() == name) {
            return false;
        }
        existingGroup = existingGroup->getNext();
    }
    return true;
}