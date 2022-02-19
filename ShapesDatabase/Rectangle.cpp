//
//  Rectangle.cpp
//  Lab5
//
//  Created by Tarek Abdelrahman on 2020-11-25.
//  Copyright © 2020 Tarek Abdelrahman.
//
//  Permission is hereby granted to use this code in ECE244 at
//  the University of Toronto. It is prohibited to distribute
//  this code, either publicly or to third parties.

//  ECE244 Student: Write the implementation of the class Rectangle here

#include <iostream>
#include <string>
#include <iomanip>
using namespace std;

#include "Shape.h"
#include "Rectangle.h"

    Rectangle::Rectangle(string n, float xcen, float ycen, float w, float h):Shape(n, xcen, ycen) {
        width = w;
        height = h;
    }
    
    // Destructor
    Rectangle::~Rectangle() {
        
    }
    
    // Accessor
    
    // Returns the width of the rectangle
    float Rectangle::getWidth() const {
        return width;
    }       
    
    // Returns the height of the rectangle
    float Rectangle::getHeight() const {
        return height;
    } 

    // Mutator
    
    // Sets the width of the rectangle
    void Rectangle::setWidth(float w) {
        width = w;
    }    
    
    // Sets the height of the rectangle
    void Rectangle::setHeight(float h) {
        height = h;
    }       
    
    // Utility methods
    
    // Draws the rectangle; for the assignment it prints the information of the rectangle
    void Rectangle::draw() const {
        // Set floating point printing to fixed point with 2 decimals
        cout << std::fixed;
        cout << std::setprecision(2);

        // Print the information
        cout << "rectangle: " << name << " " << x_centre << " " << y_centre << " " << width << " " << height << " " << computeArea() << endl;
    }    
    
    // Computes the area of the rectangle
    float Rectangle::computeArea() const {
        return (width*height);
    }
    
    // Clones the object
    Shape* Rectangle::clone() const {
        return (new Rectangle(*this));
    } 