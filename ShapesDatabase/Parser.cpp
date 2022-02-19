//
//  Parser.cpp
//  Lab5
//
//  Created by Tarek Abdelrahman on 2020-11-25.
//  Copyright © 2020 Tarek Abdelrahman.
//
//  Permission is hereby granted to use this code in ECE244 at
//  the University of Toronto. It is prohibited to distribute
//  this code, either publicly or to third parties.

// This is the main function of the program. It should include the
// command parser functions for the various shapes. One example for
// the circle shape is given, which you should mimic for the other
// shapes

// The main function does three key things
//      1. It creates a database for up to 1000 shapes and up to 10 shape types
//      2. It registers your command parser call-back functions with the database
//      3. It invokes the command parser method of the database

#include <iostream>
using namespace std;
#include <sstream>

#include "Shape.h"
#include "ShapesDB.h"

#include "Circle.h"
#include "Rectangle.h"
#include "Triangle.h"

#define MAX_SHAPES 1000
#define MAX_SHAPE_TYPES 10

// Create a database instance; making it global to simplify student code
ShapesDB sdb(MAX_SHAPES,MAX_SHAPE_TYPES);

// ECE244 Student: add your parser function prototypes here
Shape* parseCircleCommand(stringstream& line);
Shape* parseRectangleCommand(stringstream& line);
Shape* parseTriangleCommand(stringstream& line);

int main () {

    // Register the command parser call back functions
    sdb.registerShapeType("circle", &parseCircleCommand);
    sdb.registerShapeType("rectangle", &parseRectangleCommand);
    sdb.registerShapeType("triangle", &parseTriangleCommand);
    
    // Invoke the parser of the DB
    // No new commands should be registered after this
    sdb.parseCommands();
    
    // Done
    return (0);
}

Shape* parseCircleCommand(stringstream& line) {
    string name;
    float xcent;
    float ycent;
    float radius;
    line >> name >> xcent >> ycent >> radius;
    
    // Do a simple error check
    if (line.fail()) {
        cout << "Error: invalid input" << endl;
        return nullptr;
    }
    
    // Check to see if name is a reserved word
    if (sdb.isReserved(name)) {
        cout << "Error: " << name << " is a reserved word" << endl;
        return nullptr;
    }
    
    // Check if a shape with this name already exists
    if (sdb.shapeExists(name)) {
        cout << "Error: a shape with the name " << name << " already exists" << endl;
        return nullptr;
    }

    // Create the shape object and return a pointer to it
    Shape* myShape = (Shape*) new Circle(name, xcent, ycent, radius);
    cout << "created circle" << endl;
    return myShape;
}

Shape* parseRectangleCommand(stringstream& line) {
    string name;
    float xcen;
    float ycen;
    float width;
    float height;
    
    line >> name >> xcen >> ycen >> width >> height;
    
    // Do a simple error check
    if (line.fail()) {
        cout << "Error: invalid input" << endl;
        return nullptr;
    }
    
    // Check to see if name is a reserved word
    if (sdb.isReserved(name)) {
        cout << "Error: " << name << " is a reserved word" << endl;
        return nullptr;
    }
    
    // Check if a shape with this name already exists
    if (sdb.shapeExists(name)) {
        cout << "Error: a shape with the name " << name << " already exists" << endl;
        return nullptr;
    }

    // Create the shape object and return a pointer to it
    Shape* myShape = (Shape*) new Rectangle(name, xcen, ycen, width, height);
    cout << "created rectangle" << endl;
    return myShape;
}

Shape* parseTriangleCommand(stringstream& line) {
    string name;
    float x1;
    float y1;
    float x2;
    float y2;
    float x3;
    float y3;
    
    line >> name >> x1 >> y1 >> x2 >> y2 >> x3 >> y3;
    
    // Do a simple error check
    if (line.fail()) {
        cout << "Error: invalid input" << endl;
        return nullptr;
    }
    
    // Check to see if name is a reserved word
    if (sdb.isReserved(name)) {
        cout << "Error: " << name << " is a reserved word" << endl;
        return nullptr;
    }
    
    // Check if a shape with this name already exists
    if (sdb.shapeExists(name)) {
        cout << "Error: a shape with the name " << name << " already exists" << endl;
        return nullptr;
    }

    // Create the shape object and return a pointer to it
    Shape* myShape = (Shape*) new Triangle(name, x1, y1, x2, y2, x3, y3);
    cout << "created triangle" << endl;
    return myShape;
}