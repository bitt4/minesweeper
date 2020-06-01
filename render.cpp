#include <SDL2/SDL.h>

#include "include/consts.hpp"
#include "include/game.hpp"
#include "include/render.hpp"

SDL_Surface *surface = SDL_LoadBMP("bitmaps/items.bmp");
SDL_Texture *texture;

void initRender(SDL_Renderer *renderer){
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
}

void drawImage(SDL_Renderer *renderer, int x, int y, int id){

    SDL_Rect sourceRect;
    SDL_Rect destRect;

    //SDL_QueryTexture(texture, NULL, NULL, &sourceRect.w, &sourceRect.h);

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
            drawImage(renderer, j, i, 0);
        }
    }
}

void revealField(SDL_Renderer *renderer, const board_t *board){
    for(int i = 0; i < CELLS_Y; i++){
        for(int j = 0; j < CELLS_X; j++){
            drawImage(renderer, j, i, board->field[i * CELLS_X + j]);
        }
    }
}