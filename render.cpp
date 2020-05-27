#include <SDL2/SDL.h>

#include "include/render.hpp"
#include "include/consts.hpp"

void drawImage(SDL_Renderer *renderer, int x, int y, const char *filename, int id){
    SDL_Surface *surface = SDL_LoadBMP(filename);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

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
    SDL_DestroyTexture(texture);
}

void renderField(SDL_Renderer *renderer){
    for(int i = 0; i < WINDOW_H; i += CELL_WIDTH){
        for(int j = 0; j < WINDOW_W; j += CELL_WIDTH){
            drawImage(renderer, j ,i, "bitmaps/items.bmp", 0);
        }
    }
}
