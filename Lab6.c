#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define BOARDROWS 9
#define BOARDCOLS 11

bool dumpComputer = false; //causes dump of computer board before game if true (for debugging)

// boards. 0 means empty, non-zero means ship is there, number is size, -tive means hit on a ship
int playerBoard[BOARDROWS+1][BOARDCOLS+1];
int computerBoard[BOARDROWS+1][BOARDCOLS+1];
int compShotBoard[BOARDROWS+1][BOARDCOLS+1];

int getRand(int lowval,int highval){
    return (rand()%(highval+1-lowval) + lowval);
}

int getShot(bool fromUser, int board[BOARDROWS+1][BOARDCOLS+1]){
    int row,col;
    
    while(1){
        if(fromUser){
            printf("Give a shot (row, col):");
            scanf("%d%d",&row,&col);
            if(col<1 || col > BOARDCOLS || row < 1 || row > BOARDROWS)
                printf("Invalid input\n");
            else  //valid input
                break;
        }
        else { //computer generated
            row=getRand(1,BOARDROWS);
            col=getRand(1,BOARDCOLS);
            if(compShotBoard[row][col]==0) { //make sure haven't shot here before
                compShotBoard[row][col]=1; //valid shot
                break;
            }
        }
    } //will leave this loop with valid input
    if(board[row][col] != 0){
        if(board[row][col]>0)
            board[row][col]= -1*board[row][col]; //make sure is -tive
        return -1*board[row][col]; //a hit!
    }
    return 0; //miss
}
            

bool allShipsNotHit(int board[BOARDROWS+1][BOARDCOLS+1]){
    for(int i=1;i<=BOARDROWS;i++){
        for(int j=1;j<=BOARDCOLS;j++){
            if(board[i][j]>0) //ship and not hit
                return(true);
        }
    }
    return (false); //no ships found not all hit
}

bool noneLeft(int valueToFind,int board[BOARDROWS+1][BOARDCOLS+1]){
    for(int i=1;i<=BOARDROWS;i++){
        for(int j=1;j<=BOARDCOLS;j++){
            if(board[i][j]==valueToFind) //ship and not hit
                return(false);
        }
    }
    return (true); //no ships found, all hit
}

void boatOnBoard (int playerRowInput, int playerColInput, int playerOrientationInput, int boatSize, int board[BOARDROWS+1][BOARDCOLS+1]) {
    if (playerOrientationInput == 0) {
        // boatSize is equal to counter 
        for (int boat = 0; boat < boatSize; boat++) {
            board[playerRowInput][playerColInput + boat] = boatSize;
        }
    } else if (playerOrientationInput == 1) {
        for (int ship = 0; ship < boatSize; ship++) {
            board[playerRowInput + ship][playerColInput] = boatSize;
        }
    }
}

void populateBoard(bool getUserInput, int board[BOARDROWS+1][BOARDCOLS+1]){

    int playerRowInput, playerColInput, playerOrientationInput, permitted;
    int freeCount = 0;

    if (getUserInput == true) {
        printf("Rows are 1 - 9, Columns are 1 - 11\n");
        printf("Orientation is 0 for across, 1 for down\n");

        // Counter is type of ship
        for (int counter = 5; counter > 0; counter--) {
            do {
            printf("Give starting row, starting column and orientation (3 inputs) for ship of size=%d:", counter);
            scanf("%d %d %d", &playerRowInput, &playerColInput, &playerOrientationInput);
            freeCount = 0;

                if (playerRowInput <= BOARDROWS && playerRowInput > 0 && playerColInput <= BOARDCOLS && playerColInput > 0) {
                    if (playerOrientationInput == 0) {
                        if (counter + playerColInput - 1 <= BOARDCOLS) { 
                        // if this is true then program knows that the boat is within the boundaries of the board (valid input)
                            for (int side = 0; side < counter; side++) {
                                if (board[playerRowInput][playerColInput + side] == 0) {
                                    freeCount++;
                                }
                            }
                            if (freeCount == counter) {
                                permitted = 1;
                            } else {
                                permitted = 0;
                                printf("Conflicts with ship already placed\n");
                            }
                        } else {
                            permitted = 0;
                            printf("Invalid Input\n");
                        }
                    } else if (playerOrientationInput == 1) {
                        if (counter + playerRowInput - 1 <= BOARDROWS) {
                        // if this is true then program knows that the boat is within the boundaries of the board (valid input)
                            for (int vertical = 0; vertical < counter; vertical++) {
                                if (board[playerRowInput + vertical][playerColInput] == 0) {
                                    freeCount++;
                                }
                            }
                            if (freeCount == counter) {
                                permitted = 1;
                            } else {
                                permitted = 0;
                                printf("Conflicts with ship already placed\n");
                            }
                        } else {
                            permitted = 0;
                            printf("Invalid Input\n");
                        }
                    } else {
                        permitted = 0;
                        printf("Invalid Input\n");
                    }
                } else {
                    permitted = 0;
                    printf("Invalid Input\n");
                }
            }
            while (permitted == 0);
            boatOnBoard(playerRowInput, playerColInput, playerOrientationInput, counter, board);
        }
    } else {
        for (int counter = 5; counter > 0; counter--) {
            do {
                playerRowInput = getRand(1,BOARDROWS);
                playerColInput = getRand(1,BOARDCOLS);
                playerOrientationInput = getRand(0,1);

                freeCount = 0;
                if (playerOrientationInput == 0) {
                    if (counter + playerColInput - 1 <= BOARDCOLS) { 
                    // if this is true then program knows that the boat is within the boundaries of the board (valid input)
                        for (int side = 0; side < counter; side++) {
                            if (board[playerRowInput][playerColInput + side] == 0) {
                                freeCount++;
                            }
                        }
                        if (freeCount == counter) {
                            permitted = 1;
                        } else {
                            permitted = 0;
                        }
                    } else {
                        permitted = 0;
                    }
                } else if (playerOrientationInput == 1) {
                    if (counter + playerRowInput - 1 <= BOARDROWS) {
                    // if this is true then program knows that the boat is within the boundaries of the board (valid input)
                        for (int vertical = 0; vertical < counter; vertical++) {
                            if (board[playerRowInput + vertical][playerColInput] == 0) {
                                freeCount++;
                            }
                        }
                        if (freeCount == counter) {
                            permitted = 1;
                        } else {
                            permitted = 0;
                        }
                    } else {
                        permitted = 0;
                    }
                } else {
                    permitted = 0;
                }
            }
            while (permitted == 0);
            boatOnBoard(playerRowInput, playerColInput, playerOrientationInput, counter, board);
        }
    }
}

void showBoard(int board[BOARDROWS+1][BOARDCOLS+1]){
    printf("   Cols\n");
    printf("    1  2  3  4  5  6  7  8  9 10 11\n");
    printf("     ________________________________\n");
    for(int i=1;i<=BOARDROWS;i++){
        printf("R%d|",i);
        for(int j=1;j<=BOARDCOLS;j++){
            if(board[i][j]>=0)
                printf(" ");
            printf("%d ",board[i][j]);
        }
        printf("|\n");
    }
    printf("  __________________________________\n");
}

int main(int argc, char **argv)
{
    bool playerBoardOK,computerBoardOK;
    
    // initialize the seed randomly. 

     	srand(time(NULL)); 

    //init the boards
    for(int i=0;i<BOARDROWS;i++){
        for(int j=0;j<BOARDCOLS;j++){
            playerBoard[i][j]=0;
            computerBoard[i][j]=0;
            compShotBoard[i][j]=0;
        }
    }

    populateBoard(true,playerBoard);
    populateBoard(false,computerBoard);
    printf("Your board is\n");
    showBoard(playerBoard);
    if(dumpComputer){
        printf("\n\nComputer board is\n");
        showBoard(computerBoard);
    }

    // play starts here
    playerBoardOK=true;
    computerBoardOK=true;
    while(playerBoardOK && computerBoardOK){
        int hit=getShot(true,computerBoard); //shot from user
        if(hit){
            printf("HIT on size %d\n",hit);
            if(noneLeft(hit,computerBoard))
                printf("Sunk computer's %d\n",hit);
        }
        else
            printf("MISS!!\n");
        hit=getShot(false,playerBoard);
        if(hit){
            printf("Computer hits! Size=%d\n", hit);
            if(noneLeft(hit,playerBoard))
                printf("Sunk your %d\n",hit);
        }
        else
            printf("Computer miss!\n");
        playerBoardOK = allShipsNotHit(playerBoard);
        computerBoardOK = allShipsNotHit(computerBoard);
//    showBoard(playerBoard); //debug
//    showBoard(computerBoard); //debug
   }
   if(playerBoardOK)
       printf("You win!\n");
    else
        printf("You lose.\n");
    showBoard(computerBoard);

	return 0;
}
