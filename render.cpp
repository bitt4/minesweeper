#include <SDL2/SDL.h>

#include "include/render.hpp"
#include "include/consts.hpp"


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

    destRect.x = x/CELL_WIDTH*CELL_WIDTH;
    destRect.y = y/CELL_WIDTH*CELL_WIDTH;
    destRect.w = CELL_WIDTH;
    destRect.h = CELL_WIDTH;

    SDL_RenderCopy(renderer, texture, &sourceRect, &destRect);
}

void renderField(SDL_Renderer *renderer){
    for(int i = 0; i < WINDOW_H; i += CELL_WIDTH){
        for(int j = 0; j < WINDOW_W; j += CELL_WIDTH){
            drawImage(renderer, j, i, 0);
        }
    }
}
/* testing
void revealField(SDL_Renderer *renderer){
    for(int i = 0; i < WINDOW_H; i += CELL_WIDTH){
        for(int j = 0; j < WINDOW_W; j += CELL_WIDTH){
            drawImage(renderer, j, i, TriggeredMine);
        }
    }
}
*/