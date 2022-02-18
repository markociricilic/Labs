//
//  playMove.cpp
//  TicTacToe
//
//  Created by Tarek Abdelrahman on 2019-06-07.
//  Modified by Tarek Abdelrahman on 2020-09-17.
//  Copyright © 2019-2020 Tarek Abdelrahman. All rights reserved.
//
//  Permission is hereby granted to use this code in ECE244 at
//  the University of Toronto. It is prohibited to distribute
//  this code, either publicly or to third parties.
//
//  ECE244 Student: write your code for playMove in this file

#include "globals.h"
#include "GameState.h"

void playMove(GameState& game_state) {

    // game_state.set_gameBoard(game_state.get_selectedRow(), game_state.get_selectedColumn());

    if (game_state.get_gameBoard(game_state.get_selectedRow(), game_state.get_selectedColumn()) == 0) {
        game_state.set_moveValid(true);
    } else {
        game_state.set_moveValid(false);
        return;
    }
    
    if(game_state.get_turn() == true) {
        game_state.set_gameBoard(game_state.get_selectedRow(), game_state.get_selectedColumn(), X);
        game_state.set_turn(false);
    } else {
        game_state.set_gameBoard(game_state.get_selectedRow(), game_state.get_selectedColumn(), O);
        game_state.set_turn(true);
    }

    // winCode
    if ((game_state.get_gameBoard(2, 0) != Empty) && (game_state.get_gameBoard(2, 0) == game_state.get_gameBoard(1, 1)) && (game_state.get_gameBoard(2, 0) == game_state.get_gameBoard(0, 2))) {
        game_state.set_winCode(8);
        game_state.set_gameOver(true);
    } else if((game_state.get_gameBoard(0, 0) != Empty) && (game_state.get_gameBoard(0, 0) == game_state.get_gameBoard(1, 1)) && (game_state.get_gameBoard(0, 0) == game_state.get_gameBoard(2, 2))) {
        game_state.set_winCode(7);
        game_state.set_gameOver(true);
    } else if((game_state.get_gameBoard(0, 2) != Empty) && (game_state.get_gameBoard(0, 2) == game_state.get_gameBoard(1, 2)) && (game_state.get_gameBoard(0, 2) == game_state.get_gameBoard(2, 2))) {
        game_state.set_winCode(6);
        game_state.set_gameOver(true);
    } else if((game_state.get_gameBoard(0, 1) != Empty) && (game_state.get_gameBoard(0, 1) == game_state.get_gameBoard(1, 1)) && (game_state.get_gameBoard(0, 1) == game_state.get_gameBoard(2, 1))) {
        game_state.set_winCode(5);
        game_state.set_gameOver(true);
    } else if((game_state.get_gameBoard(0, 0) != Empty) && (game_state.get_gameBoard(0, 0) == game_state.get_gameBoard(1, 0)) && (game_state.get_gameBoard(0, 0) == game_state.get_gameBoard(2, 0))) {
        game_state.set_winCode(4);
        game_state.set_gameOver(true);
    } else if((game_state.get_gameBoard(2, 0) != Empty) && (game_state.get_gameBoard(2, 0) == game_state.get_gameBoard(2, 1)) && (game_state.get_gameBoard(2, 0) == game_state.get_gameBoard(2, 2))) {
        game_state.set_winCode(3);
        game_state.set_gameOver(true);
    } else if((game_state.get_gameBoard(1, 0) != Empty) && (game_state.get_gameBoard(1, 0) == game_state.get_gameBoard(1, 1)) && (game_state.get_gameBoard(1, 0) == game_state.get_gameBoard(1, 2))) {
        game_state.set_winCode(2);
        game_state.set_gameOver(true);
    } else if((game_state.get_gameBoard(0, 0) != Empty) && (game_state.get_gameBoard(0, 0) == game_state.get_gameBoard(0, 1)) && (game_state.get_gameBoard(0, 0) == game_state.get_gameBoard(0, 2))) {
        game_state.set_winCode(1);
        game_state.set_gameOver(true);
    } else {
        game_state.set_winCode(0);
        game_state.set_gameOver(true); // Draw
    }

    if(game_state.get_winCode() == 0) {
        for (int rowSize = 0; rowSize < boardSize; rowSize++) {
            for (int colSize = 0; colSize < boardSize; colSize++) {
                if (game_state.get_gameBoard(rowSize, colSize) == Empty) {
                    game_state.set_gameOver(false); 
                    return;
                }
            }
        }
    }
}