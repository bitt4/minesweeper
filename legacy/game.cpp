#include <SDL2/SDL.h>
#include <ctime>
#include <cstdlib>
#include <iostream>

#include "include/consts.hpp"
#include "include/game.hpp"
#include "include/render.hpp"

bool flags[CELLS_X*CELLS_Y];

void board_t::init(){
    
    this->state = Playing;

    srand(time(NULL));
    int randomNumber;

    for(int i = 0; i < CELLS_X*CELLS_Y; i++){
        randomNumber = rand();
        if(randomNumber%7 == 0){
            field[i] = Mine;
        }
        else{
            field[i] = Clear;
        }
    }
    
    for(int y = 0; y < CELLS_Y; y++){
        for(int x = 0; x < CELLS_X; x++){
            if(field[ y * CELLS_X + x ] != Mine)
                field[ y * CELLS_X + x ] = 15 - getNearbyMines(this, x, y);
        }
    }
}

bool flagTile(int x, int y){
    return (flags[y * CELLS_X + x] = !flags[y * CELLS_X + x]);
}

bool checkWin(const board_t *board, const bool *flags){
    bool win = true;
    for(int y = 0; y < CELLS_Y; y++){
        for(int x = 0; x < CELLS_X; x++){
            if(board->field[y * CELLS_X + x] == Mine && !flags[y * CELLS_X + x]){
                win = false;
            }
        }
    }
    
    return win;
}

int getNearbyMines(const board_t *board, int x, int y){
    int countMines = 0;

    // 3 mines above target position
    if(y - 1 >= 0){
        for(int i = -1; i < 2; i++){
            if(x+i >= 0 && x+i <= CELLS_X-1){
                if(board->field[ (y-1) * CELLS_X + x+i  ] == Mine){
                    countMines++;
                }
            }
        }
    }

    // mine left to target position
    if(x-1 >= 0){
        if(board->field[ y * CELLS_X + x-1 ] == Mine){
            countMines++;
        }
    }

    // mine right to target position
    if(x+1 <= CELLS_X-1){
        if(board->field[ y * CELLS_X + x+1 ] == Mine){
            countMines++;
        }
    }

    // 3 mines under target position
    if(y + 1 <= CELLS_Y-1){
        for(int i = -1; i < 2; i++){
            if(x+i >= 0 && x+i <= CELLS_X-1){
                if(board->field[ (y+1) * CELLS_X + x+i  ] == Mine){
                    countMines++;
                }
            }
        }
    }

    return countMines;
}

void handleClick(SDL_Renderer *renderer, board_t *board, int x, int y, int button){

    switch(button){
    case 1:
        clickOnTile(renderer, board, flags, x, y);
        break;
    case 3:
        flagTile(x, y);
        renderFlag(renderer, flags, x, y);
        if(board->state == GameOver){
            resetFlagsAndRevealed(renderer, flags);
            board->init();
            renderField(renderer);
        }
        if(checkWin(board, flags)){
            revealExceptMines(renderer, board);
            resetFlagsAndRevealed(renderer, flags);
            board->state = GameOver;
        }
        break;
    default:
        return;
    }
}