//
//  Circle.cpp
//  Lab5
//
//  Created by Tarek Abdelrahman on 2020-11-25.
//  Copyright © 2020 Tarek Abdelrahman.
//
//  Permission is hereby granted to use this code in ECE244 at
//  the University of Toronto. It is prohibited to distribute
//  this code, either publicly or to third parties.

// ***********  ECE244 Student: DO NOT MODIFY THIS FILE  ***********


#include <iostream>
#include <string>
#include <iomanip>
using namespace std;

#include "Shape.h"
#include "Circle.h"

// Build a Circle object
Circle::Circle(string n, float xcent, float ycent, float r):Shape(n, xcent, ycent) {
    radius = r;
}

Circle::~Circle() {
    // Nothing to do
}

// Accessor
float Circle::getRadius() const {
    return radius;
}

// Mutator
void Circle::setRadius(float r) {
    radius = r;
}

void Circle::draw() const {
    // Set floating point printing to fixed point with 2 decimals
    cout << std::fixed;
    cout << std::setprecision(2);
    
    // Print the information
    cout << "circle: " << name << " " << x_centre << " " << y_centre << " " << radius << " " << computeArea() << endl;
}

float Circle::computeArea() const {
    return (PI*radius*radius);
}

Shape* Circle::clone() const {
    return (new Circle(*this));
}
