//
//  Shape.h
//  Lab5
//
//  Created by Tarek Abdelrahman on 2020-11-25.
//  Copyright © 2020 Tarek Abdelrahman.
//
//  Permission is hereby granted to use this code in ECE244 at
//  the University of Toronto. It is prohibited to distribute
//  this code, either publicly or to third parties.

// ***********  ECE244 Student: DO NOT MODIFY THIS FILE  ***********

#ifndef Shape_h
#define Shape_h

#include <iostream>
#include <string>
using namespace std;

// This class is a base class for shapes in the program
// It holds variables/methods that are common to all shapes
// It is also an abstract class, so no objects of type Shape can be created
// It also contains virtual methods that must be overridden in derived classes

class Shape {
protected:
    string name;                   // The name of the shape
    float x_centre;                // The centre of the shape on the x-axis
    float y_centre;                // The centre of the shape on the y-axis

   // Constructor is protected, so it can only be invoked from derived shape
   // classes as part of building a derived object. No other classes can
   // create a Shape anyway, since it is an abstract base class
   Shape (string n, float xcent, float ycent);

public:
    // Accessors
    string getName() const;                // Returns the name of the shape
    float getXCentre() const;              // Returns centre of the shape on the x-axis
    float getYCentre() const;              // Returns centre of the shape on the y-axis

    // Mutators
    void setName(string n);                // Sets the name of the shape
    void setXCentre(float xcent);          // Sets centre of the shape on the x-axis
    void setYCentre(float ycent);          // Sets centre of the shape on the y-axis
    
    // Utility methods
    virtual void draw() const = 0 ;        // Draws a shape; for the assignment it
                                           // prints the information of the shape
    
    virtual float computeArea() const = 0; // Computes the area of the shape
    
    virtual ~Shape();                      // Virtual destructor
    
    virtual Shape* clone() const = 0;      // This method clones the object
};

#endif /* Shape_h */


