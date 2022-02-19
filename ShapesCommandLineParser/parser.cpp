//
//  parser.cpp
//  lab3
//
//  Modified by Tarek Abdelrahman on 2020-10-04.
//  Created by Tarek Abdelrahman on 2018-08-25.
//  Copyright © 2018-2020 Tarek Abdelrahman.
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

// This is the shape array, to be dynamically allocated
Shape** shapesArray;

// The number of shapes in the database, to be incremented 
int shapeCount = 0;

// The value of the argument to the maxShapes command
int max_shapes = 0;

// Prototype Functions for main
void maxShapes (stringstream &line);
void create (stringstream &line);
void move (stringstream &line);
void rotate (stringstream &line);
void draw (stringstream &line);
void deleteShapes (stringstream &line);

bool invalidName (string name);
bool invalidType (string type);
bool invalidDim (int dim);
bool invalidAngle (int angle);
bool repeatingName (string name);
bool circleDim (string type, int length1, int length2);
bool tooFewArguments (stringstream &line);
bool tooManyArguments (stringstream &line);
bool checkDecimal (stringstream &line);
bool checkNonInt (stringstream &line);

int shapeLocation (string name);

int main() {

    string line;
    string command;
    
    cout << "> ";         // Prompt for input
    getline(cin, line);    // Get a line from standard input

    while (!cin.eof()) {
        // Put the line in a linestream for parsing
        // Making a new sstream for each line so flags etc. are cleared
        stringstream lineStream (line);
        
        // Read from string stream into the command
        // The only way this can fail is if the eof is encountered
        lineStream >> command;

        // Check for the command and act accordingly
        
        bool checkCommand = false;
        
        for (int i = 0; i < 7; i++) {
            if (command == keyWordsList[i])
                checkCommand = true;
        }
        
        if (checkCommand == false) {
            cout << "Error: invalid command" << endl;
        }

        if (checkCommand) {
                if (command == "maxShapes") 
                    maxShapes(lineStream);
                else if (command == "create") 
                    create(lineStream);
                else if (command == "move") 
                    move(lineStream);
                else if (command == "rotate") 
                    rotate(lineStream);
                else if (command == "draw") 
                    draw(lineStream);
                else if (command == "delete") 
                    deleteShapes(lineStream);    
        }
        
        lineStream.clear();
        // Once the command has been processed, prompt for the next command
        cout << "> ";       // Prompt for input
        getline(cin, line);
        command = "";
    }  // End input loop until EOF.
    return 0;
}

void maxShapes (stringstream &line) { 

    if (max_shapes != 0) {
        for (int i = 0; i < max_shapes; i++) {
            delete shapesArray[i];
            shapesArray[i] = nullptr;
        }
        
        delete [] shapesArray;
        *shapesArray = nullptr;
        max_shapes = 0;
    }
    
    line >> max_shapes;
    shapesArray = new Shape*[max_shapes];

    cout << "New database: max shapes is " << max_shapes << endl;
}

void create (stringstream &line) {
    
    string name, type;
    int x_location, y_location, x_size, y_size;
        
    if (tooFewArguments(line)) {
        return;
    } 
        
    line >> name;
    
    if (line.fail()) {
        cout << "Error: invalid argument" << endl;
        return;
    } else if (invalidName(name)) {
        cout << "Error: invalid shape name" << endl;
        return;
    } else if (repeatingName(name)) {
        cout << "Error: shape " << name << " exists" << endl;
        return;
    }
    
    if (tooFewArguments(line)) {
        return;
    }
    
    line >> type;
    
    if (line.fail()) {
        cout << "Error: invalid argument" << endl;
        return;
    } else if (invalidType(type)) {
        cout << "Error: invalid shape type" << endl;
        return;
    }
    
    if (tooFewArguments(line)) {
        return;
    }
    
    line >> x_location;
    
    if (line.fail()) {
        cout << "Error: invalid argument" << endl;
        return;
    } else if (checkDecimal(line)) {
        return;
    } else if (invalidDim(x_location)) {
        cout << "Error: invalid value" << endl;
        return;
    }
    
    if (tooFewArguments(line)) {
        return;
    }
    
    line >> y_location;

    if (line.fail()) {
        cout << "Error: invalid argument" << endl;
        return;
    } else if (checkDecimal(line)) {
        return;
    } else if (invalidDim(y_location)) {
        cout << "Error: invalid value" << endl;
        return;
    }
    
    if (tooFewArguments(line)) {
        return;
    }
    
    line >> x_size;
    
    if (line.fail()) {
        cout << "Error: invalid argument" << endl;
        return;
    } else if (checkDecimal(line)) {
        return;
    } else if (invalidDim(x_size)) {
        cout << "Error: invalid value" << endl;
        return;
    }
    
    if (tooFewArguments(line)) {
        return;
    }
   
    line >> y_size;

    if (line.fail()) {
        cout << "Error: invalid argument" << endl;
        return;
    } else if (checkNonInt(line)) {
        return;
    } else if (invalidDim(y_size)) {
        cout << "Error: invalid value" << endl;
        return;
    } else if (tooManyArguments(line)) {
        return;
    }
    
    if (max_shapes == shapeCount) {
        cout << "Error: shape array is full" << endl;
        return;
    }
    
    if (!circleDim(type, x_size, y_size)) {
        
        shapesArray[shapeCount] = new Shape(name, type, x_location, y_location, x_size, y_size);
        
        shapesArray[shapeCount]->setName(name);
        shapesArray[shapeCount]->setType(type);
        shapesArray[shapeCount]->setXlocation(x_location);
        shapesArray[shapeCount]->setYlocation(y_location);
        shapesArray[shapeCount]->setXsize(x_size);
        shapesArray[shapeCount]->setYsize(y_size);
        
        shapeCount++;
        cout << "Created " << name << ": " << type << " " << x_location << " " << y_location << " " << x_size << " " << y_size << endl;
    }
}

void move (stringstream &line) {

    string name;
    int x_location, y_location;
    
    if (tooFewArguments(line)) {
        return;
    }
    
    line >> name;
    
    if (line.fail()) {
        cout << "Error: invalid argument" << endl;
        return;
    } else if (invalidName(name)) {
        cout << "Error: invalid shape name" << endl;
        return;
    } else if (!repeatingName(name)) {
        cout << "Error: shape " << name << " not found" << endl;
        return;
    }
    
    if (tooFewArguments(line)) {
        return;
    }
    
    line >> x_location;
    
    if (line.fail()) {
        cout << "Error: invalid argument" << endl;
        return;
    } else if (checkDecimal(line)) {
        return;
    } else if (invalidDim(x_location)) {
        cout << "Error: invalid value" << endl;
        return;
    }
    
    if (tooFewArguments(line)) {
        return;
    }
    
    line >> y_location;
    
    if (line.fail()) {
        cout << "Error: invalid argument" << endl;
        return;
    } else if (checkNonInt(line)) {
        return;
    } else if (invalidDim(y_location)) {
        cout << "Error: invalid value" << endl;
        return;
    }
    
    if (tooManyArguments(line)) {
        return;
    }
    
    shapesArray[shapeLocation(name)]->setXlocation(x_location);
    shapesArray[shapeLocation(name)]->setYlocation(y_location);
    
    cout << "Moved " << name << " to " << x_location << " " << y_location << endl;
}

void rotate (stringstream &line) {

    string name;
    int angle;

    if (tooFewArguments(line)) {
        return;
    }
    
    line >> name;

    if (line.fail()) {
        cout << "Error: invalid argument" << endl;
        return;
    } else if (invalidName(name)) {
        cout << "Error: invalid shape name" << endl;
        return;
    } else if (!repeatingName(name)) {
        cout << "Error: shape " << name << " not found" << endl;
        return;
    }
    
    if (tooFewArguments(line)) {
        return;
    }
    
    line >> angle;
    
    if (line.fail()) {
        cout << "Error: invalid argument" << endl;
        return;
    } else if (checkNonInt(line)) {
        return;
    } else if (invalidAngle(angle)) {
        cout << "Error: invalid value" << endl;
        return;
    }
    
    if (tooManyArguments(line)) {
        return;
    }
    
    shapesArray[shapeLocation(name)]->setRotate(angle);
    cout << "Rotated " << name << " by " << angle << " degrees" << endl;
}

void draw (stringstream &line) {
    
    if (tooFewArguments(line)) {
            return;
    }
    
    string name;
    
    line >> name;
    
    if (line.fail()) {
        cout << "Error: invalid argument" << endl;
        return;
    }
    
    if (invalidName(name) && name != "all") {
        cout << "Error: invalid shape name" << endl;
        return;
    }
    
    if (name == "all") {
        if (tooManyArguments(line)) {
            return;
        } else {
            cout << "Drew all shapes" << endl;
            
            for (int i = 0; i < shapeCount; i++) {
                if (shapesArray[i] != nullptr) {
                    shapesArray[i]->draw();
                }
            }
        }
    } else {
        
        if (!repeatingName(name)) {
            cout << "Error: shape " << name << " not found" << endl;
            return;
        }
        
        if (tooManyArguments(line)) {
            return;
        } 
        
        for (int i = 0; i < shapeCount; i++) {
            if (shapesArray[i] == nullptr) {
               continue;
            } else if ( (shapeCount != 0) && (name == shapesArray[i]->getName()) ) {
                cout << "Drew ";
                shapesArray[i]->draw();
            }
        }
    }
}

void deleteShapes (stringstream &line) {

    if (tooFewArguments(line)) {
        return;
    }
    
    string n;
    
    line >> n;
    
    if (line.fail()) {
        cout << "Error: invalid argument" << endl;
        return;
    }
    
    if (invalidName(n) && n != "all") {
        cout << "Error: invalid shape name" << endl;
        return;
    }
    
    if (n == "all") {
        if (tooManyArguments(line)) {
            return;
        } else {
            for (int i = 0; i < shapeCount; i++) {
                if (shapesArray[i] != nullptr) {
                    
                    delete shapesArray[i];
                    shapesArray[i] = nullptr;
                }
            }
            cout << "Deleted: all shapes" << endl;
            shapeCount = 0;
        }
    } else {
        
        if (!repeatingName(n)) {
            cout << "Error: shape " << n << " not found" << endl;
            return;
        } 

        if (tooManyArguments(line)) {
            return;
        }
        
        for (int i = 0; i < shapeCount; i++) {
            if (shapesArray[i] != nullptr) {
                if (n == shapesArray[i]->getName()) {
                    delete shapesArray[i];
                    shapesArray[i] = NULL;
                    
                    cout << "Deleted shape " << n << endl;
                }
            }
        }
    }
}

// Finds the shape's index in the database array
int shapeLocation (string name) {
    
    for (int i = 0; i < shapeCount; i++) {
        if (shapesArray[i] == nullptr) {
           continue;
        } else if ( (shapeCount != 0) && (name == shapesArray[i]->getName()) ) {
                return i;
        }
    }
    return -1;
}

bool repeatingName (string name) {
    
    for (int i = 0; i < shapeCount; i++) {
        if (shapesArray[i] != nullptr) {
            if ( (shapeCount != 0) && (name == shapesArray[i]->getName()) ) {
                return true;
            }
        }
    }
    return false;
}

bool invalidName (string name) {

    if (name == "all" || name == "maxShapes" || name == "create" || name == "move" || name == "rotate" || name == "draw" || name == "delete") {
        return true;
    }
    
    if (name == "circle" || name == "ellipse" || name == "rectangle" || name == "triangle") {
        return true;
    }
    
    return false;
}

bool invalidType (string type) {
    
    if (type == "circle" || type == "ellipse" || type == "rectangle" || type == "triangle") {
        return false;
    }
    return true;
}

bool invalidDim (int dim) {
    
    if (dim < 0) {
        return true;
    } else {
        return false;
    }
}

bool invalidAngle (int angle) {
    if (angle < 0 || angle > 360) {
        return true;
    } else {
        return false;
    }
}

bool circleDim (string type, int length1, int length2) {
    
    if (type == "circle" && (length1 != length2)) {
        cout << "Error: invalid value" << endl;
        return true;
    } else {
        return false;
    }
}

bool tooFewArguments (stringstream &line) {
    if ((line >> ws).eof()) {
        cout << "Error: too few arguments" << endl;
        return true;
    }
    return false;
}

bool tooManyArguments (stringstream &line) {
    if (!(line >> ws).eof()) {
        cout << "Error: too many arguments" << endl;
        return true;
    }
    return false;
}

bool checkDecimal (stringstream &line) {
    
    if (line.peek() == '.') {
        cout << "Error: invalid argument" << endl;
        return true;
    }
    return false;
}

bool checkNonInt (stringstream &line) {
    
    int check;
    
    check = line.peek();
    
    if (check != ' ' && check != '\t' && check != EOF) {
        cout << "Error: invalid argument" << endl;
        return true;
    }
    return false; 
}
