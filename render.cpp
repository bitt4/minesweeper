#include <SDL2/SDL.h>
#include <iostream>

#include "include/consts.hpp"
#include "include/game.hpp"
#include "include/render.hpp"

SDL_Surface *surface = SDL_LoadBMP((SDL_GetBasePath() + std::string("bitmaps/items.bmp")).c_str());
SDL_Texture *texture;

bool revealed[CELLS_X * CELLS_Y];

void initRender(SDL_Renderer *renderer){
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
}

void drawImage(SDL_Renderer *renderer, int x, int y, int id){

    SDL_Rect sourceRect;
    SDL_Rect destRect;

    sourceRect.x = 0;
    sourceRect.y = 16 * id;
    sourceRect.w = 16;
    sourceRect.h = 16;

    destRect.x = x*CELL_WIDTH;
    destRect.y = y*CELL_WIDTH;
    destRect.w = CELL_WIDTH;
    destRect.h = CELL_WIDTH;

    SDL_RenderCopy(renderer, texture, &sourceRect, &destRect);
}

void renderField(SDL_Renderer *renderer){
    for(int i = 0; i < CELLS_Y; i++){
        for(int j = 0; j < CELLS_X; j++){
            drawImage(renderer, j, i, Hidden);
        }
    }
}

//just for testing
void revealField(SDL_Renderer *renderer, const board_t *board){
    for(int i = 0; i < CELLS_Y; i++){
        for(int j = 0; j < CELLS_X; j++){
            drawImage(renderer, j, i, board->field[i * CELLS_X + j]);
        }
    }
}

void revealMines(SDL_Renderer *renderer, const board_t *board, const bool *flags){
    for(int y = 0; y < CELLS_Y; y++){
        for(int x = 0; x < CELLS_X; x++){
            
            if(board->field[y * CELLS_X + x] == Mine && !flags[y * CELLS_X + x]){
                drawImage(renderer, x, y, Mine);
                revealed[y * CELLS_X + x] = true;
            }
            
            if(board->field[y * CELLS_X + x] != Mine && flags[y * CELLS_X + x]){
                drawImage(renderer, x, y, FalseMine);
                revealed[y * CELLS_X + x] = true;
            }
        }
    }
    
}

void clickOnTile(SDL_Renderer *renderer, board_t *board, bool *flags, int x, int y){
    if(board->state == Playing){
        revealed[y * CELLS_X + x] = true;
        
        if(board->field[y * CELLS_X + x] == Mine){
            revealMines(renderer, board, flags);
            drawImage(renderer, x, y, TriggeredMine);
            
            resetFlagsAndRevealed(renderer, flags);
            
            board->state = GameOver;
        }
        else if(board->field[y * CELLS_X + x] == Clear){
            drawImage(renderer, x, y, Clear);
            revealGroupEmpty(renderer, board, x, y);
        }
        else {
            drawImage(renderer, x, y, board->field[y * CELLS_X + x]);
        }
    }
    else {
        board->init();
        renderField(renderer);
    }
}

void resetFlagsAndRevealed(SDL_Renderer *renderer, bool *flags){
    for(int j = 0; j < CELLS_Y; j++){
        for(int i = 0; i < CELLS_X; i++){
            revealed[j * CELLS_X + i] = false;
            flags[j * CELLS_X + i] = false;
        }
    }
}

void renderFlags(SDL_Renderer *renderer, const bool *flags){
    for(int i = 0; i < CELLS_Y; i++){
        for(int j = 0; j < CELLS_X; j++){
            if(flags[i * CELLS_X + j]){
                drawImage(renderer, j, i, Flag);
            }
        }
    }
}

void renderFlag(SDL_Renderer *renderer, const bool *flags, int x, int y){
    if(!revealed[y * CELLS_X + x]){
        if(flags[y * CELLS_X + x]){
            drawImage(renderer, x, y, Flag);
        }
        else{
            drawImage(renderer, x, y, Hidden);
        }
    }
}

int revealNearby(SDL_Renderer *renderer, const board_t *board, int x, int y){
    
    // 3 cells above target position
    int revealedCells = 0;
    
    if(y - 1 >= 0){
        for(int i = -1; i < 2; i++){
            if(x+i >= 0 && x+i <= CELLS_X-1){
                drawImage(renderer, x+i, y-1, board->field[ (y-1) * CELLS_X + x+i ]);
                revealed[ (y-1) * CELLS_X + (x+i) ] = true;
                revealedCells++;
            }
        }
    }

    // cell left to target position
    if(x-1 >= 0){
        drawImage(renderer, x-1, y, board->field[ y * CELLS_X + x-1 ]);
        revealed[ y * CELLS_X + x-1 ] = true;
        revealedCells++;
    }

    // cell right to target position
    if(x+1 <= CELLS_X-1){
        drawImage(renderer, x+1, y, board->field[ y * CELLS_X + x+1 ]);
        revealed[ y * CELLS_X + x+1 ] = true;
        revealedCells++;
    }

    // 3 cells under target position
    if(y + 1 <= CELLS_Y-1){
        for(int i = -1; i < 2; i++){
            if(x+i >= 0 && x+i <= CELLS_X-1){
                drawImage(renderer, x+i, y+1, board->field[ (y+1) * CELLS_X + x+i ]);
                revealed[ (y+1) * CELLS_X + x+i ] = true;
                revealedCells++;
            }
        }
    }
    
    return revealedCells;
}

int countRevealedNearby(int x, int y){
    // 3 cells above target position
    
    int revealedCells = 0;
    
    if(y - 1 >= 0){
        for(int i = -1; i < 2; i++){
            if(x+i >= 0 && x+i <= CELLS_X-1){
                revealedCells += revealed[ (y-1) * CELLS_X + (x+i) ];
            }
        }
    }

    // cell left to target position
    if(x-1 >= 0){
        revealedCells += revealed[ y * CELLS_X + x-1 ];
    }

    // cell right to target position
    if(x+1 <= CELLS_X-1){
        revealedCells += revealed[ y * CELLS_X + x+1 ];
    }

    // 3 cells under target position
    if(y + 1 <= CELLS_Y-1){
        for(int i = -1; i < 2; i++){
            if(x+i >= 0 && x+i <= CELLS_X-1){
                revealedCells += revealed[ (y+1) * CELLS_X + x+i ];
            }
        }
    }
    
    return revealedCells;
}

void revealGroupEmpty(SDL_Renderer *renderer, const board_t *board, int x, int y){
    
    int revealedIter = 0, countClearRevealed;
    
    if(board->field[ y * CELLS_X + x ] == Clear && revealed[ y * CELLS_X + x ]){
        revealNearby(renderer, board, x, y);
    }
    
    while(true) {

        countClearRevealed = 0;

        for(int i = 0; i < CELLS_Y; i++){
            for(int j = 0; j < CELLS_X; j++){
                if(board->field[ i * CELLS_X + j ] == Clear && revealed[ i * CELLS_X + j ]) {
                    countClearRevealed++;
                    revealNearby(renderer, board, j, i);
                }
            }
        }
        if(countClearRevealed == revealedIter){
            break;
        }
        revealedIter = countClearRevealed;
    }
}

void revealEmpty(SDL_Renderer *renderer, const board_t *board){
    for(int i = 0; i < CELLS_Y; i++){
        for(int j = 0; j < CELLS_X; j++){
            if(board->field[i * CELLS_X + j] == Clear){
                drawImage(renderer, j, i, Clear);
            }
        }
    }
}

void revealExceptMines(SDL_Renderer *renderer, const board_t *board){
    for(int y = 0; y < CELLS_Y; y++){
        for(int x = 0; x < CELLS_X; x++){
            if(board->field[y * CELLS_X + x] != Mine){
                drawImage(renderer, x, y, board->field[y * CELLS_X + x]);
            }
        }
    }
}