//
//  ShapesDB.h
//  Lab5
//
//  Created by Tarek Abdelrahman on 2020-11-25.
//  Copyright © 2020 Tarek Abdelrahman.
//
//  Permission is hereby granted to use this code in ECE244 at
//  the University of Toronto. It is prohibited to distribute
//  this code, either publicly or to third parties.

// ***********  ECE244 Student: DO NOT MODIFY THIS FILE  ***********

#ifndef ShapesDB_h
#define ShapesDB_h

#include <iostream>
using namespace std;
#include <sstream>

#include "Shape.h"

// A structure to hold the names of the shapes and their associated command parser functions
struct registered_shape_types {
    string shape_name;
    Shape* (*parser_function)(stringstream&);
};

class ShapesDB {
private:
    int maxShapes;                                   // The size of the array DB
    int maxShapeTypes;                               // The number of shape types
    Shape** shapesArray;                             // The pointers-to-Shape array
    int shapesCount = 0;                             // Number of Shape objects in DB
    int typesCount = 0;                              // The number of registered shape types
 
    struct registered_shape_types* registeredShapes; // Registered shape types array
    
    // This is a private data member used to facilitate iteration
    int current = 0;
    
    // These are private helper functions
    void handleDrawCommand() const;
    void handleAreaCommand() const;
    
    // Do not pass a ShapesDB by value
    ShapesDB(const ShapesDB& src) = delete;
    
public:
    ShapesDB(int n, int c);                          // Create DB for n shapes and c shape types
    virtual ~ShapesDB();                             // De-allocate the DB
    
    // Register a new shape type using a string for the type name (e.g., circle,
    // rectangle or triangle) and a pointer to a user function that parses the
    // command for this shape type (e.g., circle my_circ x_cent y_cent radius)
    bool registerShapeType(string shapeName,
                         Shape* (*shapeCommandParser)(stringstream& linestream));
    
    // Parse commands from the until an EOF is encountered
    // When the command creates a shape, the user ShapeCommandParser function returns
    // a pointer to a shape object of the correct type. This function adds the Shape
    // object to the DB.
    void parseCommands();
    
    // Finds a shape with the name n and returns a pointer to a *copy* of the object
    // or a nullptr if the object does not exist. The user is responsible for deleting
    // the copy once done using it
    Shape* findShape(string n) const;
    
    // Updates the DB with the shape pointed to by shape_ptr (i.e., *shape_ptr)
    // If an object with the same name as that of *shape_ptr exists, the existing
    // object is updated from *shape_ptr. If no object with the same name exists,
    // *shape_ptr is copied and the copy is inserted into the DB. The user may delete
    // *shape_ptr afterwards. The return value is true if the object is updated and
    // false if the object is inserted.
    bool updateShape(Shape* shape_ptr);
    
    // Checks if a shape with the name n exists
    bool shapeExists(string n) const;
    
    // Checks if the DB is full
    bool isFull() const;
    
    // Checks of the name n is a reserved word (command or shape type)
    bool isReserved(string n) const;
    
    // The following methods implement a very rudimentary iterator
    // It is not very robust, but gives one the ability to loop over
    // all the shapes in the DB
    
    // Get a pointer to the first shape in the DB
    Shape* begin();
    
    // Get a pointer to the last shape in the DB
    Shape* end();
    
    // Advance the iterator to get a pointer to the next shape in the DB
    Shape* next();
    
    // Here is how you should use this iterator; sdb is the name of a ShapesDB object
    // for (Shape* iter = sdb.begin(); iter != sdb.end(); iter = sdb.next()) {
    //      if (iter != nullptr) cout << iter->getName() << endl;
    // }
};

#endif /* ShapesDB_h */
