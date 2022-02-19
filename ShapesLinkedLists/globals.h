//
//  globals.h
//  Lab4
//
//  Created by Tarek Abdelrahman on 2020-10-25.
//
//  Permission is hereby granted to use this code in ECE244 at
//  the University of Toronto. It is prohibited to distribute
//  this code, either publicly or to third parties.


#ifndef globals_h
#define globals_h

//  Copyright © 2020 Tarek Abdelrahman.
//
//  Permission is hereby granted to use this code in ECE244 at
//  the University of Toronto. It is prohibited to distribute
//  this code, either publicly or to third parties.

// This file should be included only in Parser.cpp

#include <iostream>
using namespace std;
#include<string>

// These arrays should make it easier to check that a name is not a reserved word
// and that an entered shape type is valid
#define NUM_KEYWORDS 6
string keyWordsList[NUM_KEYWORDS]={"shape", "group", "move", "delete", "draw", "pool"};

#define NUM_TYPES 3
string shapeTypesList[NUM_TYPES]={"ellipse", "rectangle", "triangle"};
#endif /* globals_h */

