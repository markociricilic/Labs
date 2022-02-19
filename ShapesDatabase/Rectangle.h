//
//  Rectangle.h
//  Lab5
//
//  Created by Tarek Abdelrahman on 2020-11-25.
//  Copyright © 2020 Tarek Abdelrahman.
//
//  Permission is hereby granted to use this code in ECE244 at
//  the University of Toronto. It is prohibited to distribute
//  this code, either publicly or to third parties.

#ifndef Rectangle_h
#define Rectangle_h

#include <iostream>
#include <string>
using namespace std;


// This class implements the rectangle Shape objects.
// ECE244 Student: Write the definition of the class here.

#include "Shape.h"

class Rectangle : Shape {
private:
    float width;                  // The width of the rectangle
    float height;                 // The length of the rectangle

public:
    // Constructor
    Rectangle (string n, float xcen, float ycen, float w, float h);
    
    // Destructor
    virtual ~Rectangle();
    
    // Accessor
    float getWidth() const;       // Returns the width of the rectangle
    float getHeight() const;       // Returns the height of the rectangle

    // Mutator
    void setWidth(float w);       // Sets the width of the rectangle
    void setHeight(float h);       // Sets the height of the rectangle
    
    // Utility methods
    virtual void draw() const;     // Draws the rectangle; for the assignment it prints the information of the rectangle
    
    virtual float computeArea() const;   // Computes the area of the rectangle
    
    virtual Shape* clone() const;  // Clones the object
};

#endif /* Rectangle_h */