/*
 * File:   project_reversi_skeleton.h
 * Author: APS105H1 Teaching Team
 *
 * Date: Jan 2021
 */

// NOTE: Do NOT change this file, add more functions in your C file
#ifndef SKELETON_H
#define SKELETON_H

#include <stdbool.h>

// Function Declrations for part 1 of the project
void printBoard(char board[][26], int n);
bool positionInBounds(int n, int row, int col);
bool checkLegalInDirection(char board[][26], int n, int row,
 		int col, char colour, int deltaRow, int deltaCol);

// Function Declration for part 2 of the project
int makeMove(const char board[26][26], int n, char turn, int *row, int *col);

#endif
