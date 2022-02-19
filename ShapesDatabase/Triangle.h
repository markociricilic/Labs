//
//  Triangle.h
//  Lab5
//
//  Created by Tarek Abdelrahman on 2020-11-25.
//  Copyright © 2020 Tarek Abdelrahman.
//
//  Permission is hereby granted to use this code in ECE244 at
//  the University of Toronto. It is prohibited to distribute
//  this code, either publicly or to third parties.

#ifndef Triangle_h
#define Triangle_h

#include <iostream>
#include <string>
using namespace std;


// This class implements the triangle Shape objects.
// ECE244 Student: Write the definition of the class here.

#include "Shape.h"

class Triangle : Shape {
private:
    float x_coord1;                  // The x1 of the triangle
    float y_coord1;                 // The y1 of the triangle
    float x_coord2;                 // The x2 of the triangle
    float y_coord2;                 // The y2 of the triangle
    float x_coord3;                 // The x3 of the triangle
    float y_coord3;                 // The y3 of the triangle

public:
    // Constructor
    Triangle (string n, float x1, float y1, float x2, float y2, float x3, float y3);
    
    // Destructor
    virtual ~Triangle();
    
    // Accessor
    float getX1() const;       // Returns the x1 of the triangle
    float getY1() const;       // Returns the y1 of the triangle
    float getX2() const;       // Returns the x2 of the triangle
    float getY2() const;       // Returns the y2 of the triangle
    float getX3() const;       // Returns the x3 of the triangle
    float getY3() const;       // Returns the y3 of the triangle

    // Mutator
    void setX1(float x1);       // Sets the x1 of the triangle
    void setY1(float y1);       // Sets the y1 of the triangle
    void setX2(float x2);       // Sets the x2 of the triangle
    void setY2(float y2);       // Sets the y2 of the triangle
    void setX3(float x3);       // Sets the x3 of the triangle
    void setY3(float y3);       // Sets the y3 of the triangle
    
    // Utility methods
    virtual void draw() const;     // Draws the rectangle; for the assignment it prints the information of the rectangle
    
    virtual float computeArea() const;   // Computes the area of the rectangle
    
    virtual Shape* clone() const;  // Clones the object
};

#endif /* Triangle_h */

