/*
 * File:        project_reversi_skeleton.c
 * Author:      APS105H1 Teaching Team
 * Modified by: Marko Ciric
 * Student Number: 1006723967
 * 
 * Date: Jan 2021
 */

#include <stdio.h>
#include "project_reversi_skeleton.h" // DO NOT modify this line

// ========= Function implementation ==========
/*
 * Function:  printBoard 
 * --------------------
 * Print the whole board to the terminal window.
 */
void printBoard(char board[][26], int n) {

    printf("  "); // For aligning columns with start of board

    // Prints the letter labels at the top of array (columns)
    for (int countColLabel = 0; countColLabel < n; countColLabel++) {
        printf("%c",countColLabel + 97); // Using a ASCII Table starts at 97 ('a') and continues --> 'b' --> 'c' until user 'n'
    }
    printf("\n");

    // Prints the board
    for (int countRow = 0; countRow < n; countRow++) {
        printf("%c ",countRow + 97); // Prints the letter labels at the side of array (rows)
        for (int countCol = 0; countCol < n; countCol++) {
            printf("%c",board[countRow][countCol]);
        }
        printf("\n");
    }
}

/*
 * Function:  initializingBoard 
 * --------------------
 * Prints the initial occupied board array elements.
 */
void initializingBoard(char board[][26], int n) {

    for (int unoccupiedRow = 0; unoccupiedRow < n; unoccupiedRow++) {
        for (int unoccupiedCol = 0; unoccupiedCol < n; unoccupiedCol++) {
            board[unoccupiedRow][unoccupiedCol] = 'U'; // Setting all array elements as 'U'; meaning empty
        }
    }

    // Setting array elements for initial 'WBWB' at center overwriting the 'U'
    // W = occupied by white (will always be on center diagonal of the board; row and column will be SAME)
    // B = occupied by black (to sides of the initial white occupied space)
    board[n/2][n/2] = 'W';
    board[n/2][n/2 - 1] = 'B';
    board[n/2 - 1][n/2 - 1] = 'W';
    board[n/2 - 1][n/2] = 'B';
}

/*
 * Function:  positionInBounds 
 * --------------------
 * Checks whether the specified (row, col) lies within the board dimensions.
 */
bool positionInBounds(int n, int row, int col) {

    // Checking row and column for valid conditions
    if (row < n && row >= 0 && col < n && col >= 0) {
        return true;
    }
    return false;
}

/*
 * Function:  userBoardConfiguration 
 * --------------------
 * Redefines the board according to the user's input.
 */
void userBoardConfiguration(char board[][26], char userInput[4], int n) {

    // userInput[1] == row AND userInput[2] == column
    if (positionInBounds(n, userInput[1] - 97, userInput[2] - 97)) {
        board[userInput[1] - 97][userInput[2] - 97] = userInput[0]; // userInput[0] == colour
    }
}

bool secondCheckLegalInDirection(char board[][26], int n, int row, int col, char colour, int deltaRow, int deltaCol) {

    /* Calls itself recursively for the opposite colour until the player's colour is found OR if out-of-bounds OR there is empty 
    space. The exit cases are true if player's colour is found and false if out-of-bounds or empty space is present. */

    if (positionInBounds(n, row, col)) {
        if (board[row][col] != 'U' && board[row][col] != colour) {
            return secondCheckLegalInDirection(board, n, row+deltaRow, col+deltaCol, colour, deltaRow, deltaCol);
        } else if (board[row][col] == colour) {
            return true;
        }
    }
    return false;
}

/*
 * Function:  checkLegalInDirection
 * --------------------
 * Checks whether (row, col) is a legal position for a tile of colour by “looking” in the direction 
 * specified by deltaRow and deltaCol.
 */
bool checkLegalInDirection(char board[][26], int n, int row, int col, char colour, int deltaRow, int deltaCol) {

    // Keeps on checking further after space is checked
    row += deltaRow;
    col += deltaCol;

    if (positionInBounds(n, row, col)) {
        /* Checks whether there is the opposite colour adjacent to the space in the direction being checked by checking there
        is no empty space and player's colour is not present. */
        if(board[row][col] != 'U' && board[row][col] != colour) {
            // Checks the second function now searching for player's colour not opposite colour 
            return secondCheckLegalInDirection(board, n, row+deltaRow, col+deltaCol, colour, deltaRow, deltaCol);
        }
    }
    return false;             
}

/*
 * Function:  displayValidMoves 
 * --------------------
 * Checks when input is within bounds, follows the 'checkLegalInDirection', and if the space being checked IS free (== 'U'), then 
 * the MIN is '-1' while the MAX is '1' for direction so row AND column must be between (-1,1). There CANNOT be input (0,0), so 
 * only valid is otherwise.
 */
void displayValidMoves(char board[][26], int n, char colour) {

    for (int unoccupiedRow = 0; unoccupiedRow < n; unoccupiedRow++) {
        for (int unoccupiedCol = 0; unoccupiedCol < n; unoccupiedCol++) {
            if (board[unoccupiedRow][unoccupiedCol] == 'U') {
                for (int deltaRow = -1; deltaRow <= 1; deltaRow++) {
                    for (int deltaCol = -1; deltaCol <= 1; deltaCol++) {
                        if (deltaRow != 0 || deltaCol != 0) {
                            if (checkLegalInDirection(board, n, unoccupiedRow, unoccupiedCol, colour, deltaRow, deltaCol)) {
                                printf("%c%c\n", unoccupiedRow + 97, unoccupiedCol + 97);

                                // Prevents checking the same space when legal direction is already found (+= 2 or greater is valid)
                                deltaRow += 69;
                                deltaCol += 69;
                            }
                        }
                    }
                }
            }
        }
    }
}

/*
 * Function:  flip 
 * --------------------
 * Checks tokens in further directions after the one placed down - flipping them until its same colour is 
 * located on the opposite end (from the while loop) then stopping.
 */
void flip(char board[][26], int n, int row, int col, char colour, int deltaRow, int deltaCol) {

    // These place down the new token
    board[row][col] = colour;

    row += deltaRow;
    col += deltaCol;

    // While loop flips the existing tokens
    while (board[row][col] != colour) {
        board[row][col] = colour;

        row += deltaRow;
        col += deltaCol;
    }
}

/*
 * Function:  checkingMoveValid 
 * --------------------
 * Partly from 'displayValidMoves', checks whether the player's input is a valid input. Then flips the token. 
 */
void checkingMoveValid(char board[][26], int n, int row, int col, char colour) {

    int counter = 0;

    if (positionInBounds(n, row, col)) {
        if (board[row][col] == 'U') {
            for (int deltaRow = -1; deltaRow <= 1; deltaRow++) {
                for (int deltaCol = -1; deltaCol <= 1; deltaCol++) {
                    if (deltaRow != 0 || deltaCol != 0) {
                        if (checkLegalInDirection(board, n, row, col, colour, deltaRow, deltaCol)) {
                            flip(board, n, row, col, colour, deltaRow, deltaCol);
                            counter++;
                        }
                    }
                }
            }
        }
    }

    if (counter > 0) {
        printf("Valid move.\n");
    } else {
        printf("Invalid move.\n");
    }
}

/*
 * Function:  makeMove 
 * --------------------
 * The player "turn" makes a move at (row, col).
 * Note: This function MUST NOT change board, only row and col can be changed to reflect the move.
 */
int makeMove(const char board[26][26], int n, char turn, int *row, int *col) {
    (void)board;
    (void)n;
    (void)turn;
    (void)row;
    (void)col;
    return 0;
}

#ifndef COMPETE_MODE // DO NOT DELETE THIS LINE
int main(void) {

    char board[26][26]; // Declaring a static 2D-Array; MAX assumed board dimensions

    int n; // User board input
    char userInput[4]; // For three inputs (colour, row, col, ...)

    printf("Enter the board dimension: ");
    scanf("%d",&n);

    initializingBoard(board, n);
    printBoard(board, n);

    printf("Enter board configuration:\n");
    scanf("%s",userInput);
    userBoardConfiguration(board, userInput, n);

    // User must input '!!!' in order to finish configuring the board
    while (userInput[0] != '!' || userInput[1] != '!' || userInput[2] != '!') {
        scanf("%s",userInput);
        userBoardConfiguration(board, userInput, n);
    }

    printBoard(board, n); // Prints the board again with user's configuration

    printf("Available moves for W:\n");
    displayValidMoves(board, n, 'W');

    printf("Available moves for B:\n");
    displayValidMoves(board, n, 'B');


    printf("Enter a move:\n");
    scanf("%s", userInput);

    // Prints the board with the user's valid input
    checkingMoveValid(board, n, userInput[1] - 97, userInput[2] - 97, userInput[0]);
    printBoard(board, n);

    return 0;
}
#endif // DO NOT DELETE THIS LINE
