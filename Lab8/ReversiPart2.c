/*
 * File:        project_reversi_skeleton.c
 * Author:      APS105H1 Teaching Team
 * Modified by: Marko Ciric
 * Student Number: 1006723967
 * 
 * Date: Jan 2021
 */

#include <stdio.h>
// #include "project_reversi_skeleton.h" // DO NOT modify this line

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
bool displayValidMoves(char board[][26], int n, char colour) {

    for (int unoccupiedRow = 0; unoccupiedRow < n; unoccupiedRow++) {
        for (int unoccupiedCol = 0; unoccupiedCol < n; unoccupiedCol++) {
            if (board[unoccupiedRow][unoccupiedCol] == 'U') {
                for (int deltaRow = -1; deltaRow <= 1; deltaRow++) {
                    for (int deltaCol = -1; deltaCol <= 1; deltaCol++) {
                        if (deltaRow != 0 || deltaCol != 0) {
                            if (checkLegalInDirection(board, n, unoccupiedRow, unoccupiedCol, colour, deltaRow, deltaCol)) {
                                // Counts the number of legal moves
                                return true;

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
    // If no legal move found return false
    return false;
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
 * Function:  countFlip 
 * --------------------
 * Determines the number of times a token is flipped by a move.
 */
int countFlip(char board[][26], int n, int row, int col, char colour, int deltaRow, int deltaCol) {

    int flipCounter = 0;

    row += deltaRow;
    col += deltaCol;

    // While loop counts the number of token that can be flipped in this direction
    while (board[row][col] != colour) {
        flipCounter++;

        row += deltaRow;
        col += deltaCol;
    }
    return flipCounter;
}

/*
 * Function:  checkingMoveValid 
 * --------------------
 * Partly from 'displayValidMoves', checks whether the player's input is a valid input. Then flips the token.
 */
int checkingMoveValid(char board[][26], int n, int row, int col, char colour) {

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

    if (counter == 0) {
        printf("Invalid move.\n");
    }

    return counter;
}

/*
 * Function:  movePriority 
 * --------------------
 * Certain sections of the board will have greater priority in terms of best move. This function will evaluate the available valid
 * moves for the player, and aim to place a token in order of priority. The best space is referred to as '1', and the least 
 * favourable space is referred to as '5'. (to access these sections all elements will be baed on the input 'n').
 * 
 * 1 - The 4 end corners of the board
 * 2 - The 'center' square of the board (not the initial placement, but surrounding that) 
 * 3 - The 'center' edges of the board
 * 4 - The 'deeper center' edges of the board
 * 5 - The squares at the corners of the board EXCLUDING priority '1'
 */
int movePriority(char board[][26], int n, int row, int col, char colour) {

    int moveRank;

    if (board[row][col] == 'U') {
        if ((row == 0 && col == 0) || (row == 0 && col == n - 1) || (row == n - 1 && col == 0) || (row == n - 1 && col == n - 1 )) {
            moveRank = 1;
            return moveRank;
        } else if ((row >= 0 && row < 2 && col >= 0 && col < 2) || (row >= 0 && row < 2 && col >= n - 2 && col < n) || (row >= n - 2 && row < n && col >= 0 && col < 2) || (row >= n - 2 && row < n && col >= n - 2 && col < n)) {
            moveRank = 5;
            return moveRank;
        } else if ((row == 0) || (row == n - 1) || (col == 0) || (col == n - 1)) {
            moveRank = 3;
            return moveRank;
        } else if ((n >= 8 && row >= n/2 - 2 && row <= n/2 + 1 && col >= n/2 - 2 && col <= n/2 + 1)) {
            moveRank = 2;
            return moveRank;
        } else {
            moveRank = 4;
            return moveRank;
        }
    }
}

/*
 * Function:  evaluateComputerMove 
 * --------------------
 * Counts the number of tokens the computer can flip in each direction for a specific move (while is still legal move).
 */
int evaluateComputerMove(char board[][26], int n, int row, int col, char colour) {

    int counter = 6;

    if (positionInBounds(n, row, col)) {
        if (board[row][col] == 'U') {
            for (int deltaRow = -1; deltaRow <= 1; deltaRow++) {
                for (int deltaCol = -1; deltaCol <= 1; deltaCol++) {
                    if (deltaRow != 0 || deltaCol != 0) {
                        if (checkLegalInDirection(board, n, row, col, colour, deltaRow, deltaCol)) {
                            return movePriority(board, n, row, col, colour);
                        }
                    }
                }
            }
        }
    }
    return counter;
}

/*
 * Function:  makeMove 
 * --------------------
 * The player "turn" makes a move at (row, col).
 * Note: This function MUST NOT change board, only row and col can be changed to reflect the move.
 */
int makeMove(const char board[26][26], int n, char turn, int *row, int *col) {

    int moveRank = 6;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (evaluateComputerMove(board, n, i, j, turn) < moveRank && board[i][j] == 'U') {
                moveRank = evaluateComputerMove(board, n, i, j, turn);
                *row = i;
                *col = j;
            }
        }
    }
    return 0;
}

#ifndef COMPETE_MODE // DO NOT DELETE THIS LINE
int main(void) {

    char board[26][26]; // Declaring a static 2D-Array; MAX assumed board dimensions

    int n; // User board input
    char userInput[3]; // For two inputs (row, col)
    char colourComputer;
    char colourPlayer;
    int counterB = 0; // Number of black tokens at end of game
    int counterW = 0; // Number of white tokens at end of game
    int row;
    int col;
    int turns = 0; // Counts every time someone places a token
    int invalidInput = 0; // Becomes 1 if user has invalid input

    printf("Enter the board dimension: ");
    scanf("%d",&n);

    printf("Computer plays (B/W) : ");
    scanf(" %c", &colourComputer);

    initializingBoard(board, n);
    printBoard(board, n);

    // Ensures that the player colour is opposite the computers
    if (colourComputer == 'B') {
        colourPlayer = 'W';
    } else {
        colourPlayer = 'B';
    }

    // While loop continues as long as there is a valid move for EITHER colour; breaks once no valid move found
    while (displayValidMoves(board, n, 'B') || displayValidMoves(board, n, 'W')) {
        if (displayValidMoves(board, n, 'B')) {
            if (colourComputer == 'B') {
                makeMove(board, n, colourComputer, &row, &col);
                checkingMoveValid(board, n, row, col, colourComputer);

                // Prints the computer's move
                printf("Computer places %c at %c%c.\n", colourComputer, row + 97, col + 97);
            } else if (colourPlayer == 'B') {
                printf("Enter move for colour %c (RowCol): ", colourPlayer);
                scanf("%s", userInput); // The input is for the row and column (as a string)

                // Calling this function would execute the player's move, if it returns as 0 (no valid move) then computer wins
                if (checkingMoveValid(board, n, userInput[0] - 97, userInput[1] - 97, colourPlayer) == 0) {
                    printf("%c player wins.\n", colourComputer);
                    invalidInput++;
                    break;
                }
            } 
            turns++;
            printBoard(board, n);

            if (turns == n*n - 4) {
                break;
            }
        } else {
            printf("B player has no valid move.\n");
        }

        if (displayValidMoves(board, n, 'W')) {
            if (colourComputer == 'W') {
                makeMove(board, n, colourComputer, &row, &col);
                checkingMoveValid(board, n, row, col, colourComputer);

                // Prints the computer's move
                printf("Computer places %c at %c%c.\n", colourComputer, row + 97, col + 97);
            } else if (colourPlayer == 'W') {
                printf("Enter move for colour %c (RowCol): ", colourPlayer);
                scanf("%s", userInput); // The input is for the row and column (as a string)

                // Calling this function would execute the player's move, if it returns as 0 (no valid move) then computer wins
                if (checkingMoveValid(board, n, userInput[0] - 97, userInput[1] - 97, colourPlayer) == 0) {
                    printf("%c player wins.\n", colourComputer);
                    invalidInput++;
                    break;
                }
            } 
            turns++;
            printBoard(board, n);

            if (turns == n * n - 4) {
                break;
            }
        } else {
            printf("W player has no valid move.\n");
        }
    }

    if (invalidInput == 0) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (board[i][j] == 'W') {
                    counterW++;
                } else if (board[i][j] == 'B') {
                    counterB++;
                }
            }
        }

        // In case of tie (white and black tokens have equal amounts)
        if (counterW == counterB) {
            printf("Draw!\n");
        } else if (counterW > counterB) {
            printf("W player wins.\n");
        } else if (counterB > counterW) {
            printf("B player wins.\n");
        }
    }

    return 0;
}
#endif // DO NOT DELETE THIS LINE