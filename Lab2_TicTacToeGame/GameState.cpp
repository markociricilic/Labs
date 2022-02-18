//
//  GameState.cpp
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

#include "globals.h"
#include "GameState.h"

    GameState::GameState() {
        int selectedRow = 0;  
        int selectedColumn = 0; 
        bool moveValid = true;    
        bool gameOver = false;      
        int winCode = 0;       
        bool turn = true;

        for (int rowSize = 0; rowSize < boardSize; rowSize++) {
            for (int colSize = 0; colSize < boardSize; colSize++) {
                gameBoard[rowSize][colSize] = Empty;
            }
        }
    };

    int GameState::get_selectedRow() {
        return selectedRow;
    }     

    int GameState::get_selectedColumn() {
        return selectedColumn;
    } 

    void GameState::set_selectedRow(int value) {
        if(value >= 0 && value < boardSize) 
            selectedRow = value;
    }     

    void GameState::set_selectedColumn(int value) {
        if(value >= 0 && value < boardSize) 
            selectedColumn = value;
    }  

    bool GameState::get_moveValid() {
        return moveValid;
    }

    void GameState::set_moveValid(bool value) {
        moveValid = value;
    }

    bool GameState::get_gameOver() {
        return gameOver;
    }

    void GameState::set_gameOver(bool value) {
        gameOver = value;
    }

    int GameState::get_winCode() {
        return winCode;
    }
    
    void GameState::set_winCode(int value) {
        if(value >= 0 && value <= 8) 
            winCode = value;
    }
    
    bool GameState::get_turn() {
        return turn;
    }

    void GameState::set_turn(bool value) {
        turn = value;
    }

    int GameState::get_gameBoard(int row, int col) {
        if(row >= 0 && row < boardSize && col >= 0 && col < boardSize) {
            return gameBoard[row][col];
        } else {
            return Empty;
        }
    } 

    void GameState::set_gameBoard(int row, int col, int value) {
        if(row >= 0 && row < boardSize && col >= 0 && col < boardSize) {
            gameBoard[row][col] = value;
        } else {
            return;
        }
    }