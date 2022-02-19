//
//  Triangle.cpp
//  Lab5
//
//  Created by Tarek Abdelrahman on 2020-11-25.
//  Copyright © 2020 Tarek Abdelrahman.
//
//  Permission is hereby granted to use this code in ECE244 at
//  the University of Toronto. It is prohibited to distribute
//  this code, either publicly or to third parties.

//  ECE244 Student: Write the implementation of the class Triangle here

#include <iostream>
#include <string>
#include <iomanip>
#include <cmath>
using namespace std;

#include "Shape.h"
#include "Triangle.h"


    // Build a Triangle Object
    Triangle::Triangle (string n, float x1, float y1, float x2, float y2, float x3, float y3):Shape(n, ((x1 + x2 + x3)/3), ((y1 + y2 + y3)/3)) {
        x_coord1 = x1;        
        y_coord1 = y1;      
        x_coord2 = x2;                 
        y_coord2 = y2;               
        x_coord3 = x3;               
        y_coord3 = y3;                
    }

        // Destructor
    Triangle::~Triangle() {

    }

        // Accessor
    // Returns the x1 of the triangle
    float Triangle::getX1() const {
        return x_coord1;
    }      

    // Returns the y1 of the triangle
    float Triangle::getY1() const {
        return y_coord1;
    }

    // Returns the x2 of the triangle
    float Triangle::getX2() const {
        return x_coord2;
    }     

    // Returns the y2 of the triangle
    float Triangle::getY2() const {
        return y_coord2;
    }       

    // Returns the x3 of the triangle
    float Triangle::getX3() const {
        return x_coord3;
    }      

    // Returns the y3 of the triangle
    float Triangle::getY3() const {
        return y_coord3;
    }      

    // Mutator

    // Sets the x1 of the triangle
    void Triangle::setX1(float x1) {
        x_coord1 = x1;
    }

    // Sets the y1 of the triangle
    void Triangle::setY1(float y1) {
        y_coord1 = y1;
    }    

    // Sets the x2 of the triangle
    void Triangle::setX2(float x2) {
        x_coord2 = x2;
    }   

    // Sets the y2 of the triangle
    void Triangle::setY2(float y2) {
        y_coord2 = y2;
    }    

    // Sets the x3 of the triangle
    void Triangle::setX3(float x3) {
        x_coord3 = x3;
    }     

    // Sets the y3 of the triangle
    void Triangle::setY3(float y3) {
        y_coord3 = y3;
    }     

        // Utility methods

    // Draws the triangle; for the assignment it prints the information of the triangle
    void Triangle::draw() const {
        // Set floating point printing to fixed point with 2 decimals
        cout << std::fixed;
        cout << std::setprecision(2);

        // Print the information
        cout << "triangle: " << name << " " << x_centre << " " << y_centre << " " << x_coord1 << " " << y_coord1 << " " << x_coord2 << " " << y_coord2 << " " << x_coord3 << " " << y_coord3 << " " << computeArea() << endl;
    }   

    // Computes the area of the triangle
    float Triangle::computeArea() const {
        return (abs((x_coord1*(y_coord2 - y_coord3) + x_coord2*(y_coord3 - y_coord1) + x_coord3*(y_coord1 - y_coord2)) / 2));
    }

    // Clones the object
    Shape* Triangle::clone() const {
        return (new Triangle(*this));
    }