#include <SDL2/SDL.h>

#include <Windows.h>
#include <iostream>
#include <cstdio>

#define DEBUG

//#define WINDOW_H 401
#include "include/consts.hpp"
#include "include/handle_click.hpp"
#include "include/render.hpp"

int main(int argc, char *argv[]){

#ifdef DEBUG
    AllocConsole();
    std::freopen("CON", "w", stdout);
#endif

    if(SDL_Init(SDL_INIT_EVERYTHING) != 0){
        std::cout << "Init failed\n";
        return EXIT_FAILURE;
    }

    std::cout << "Init successful\n";

    SDL_Window *window = SDL_CreateWindow("Minesweeper",
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          WINDOW_W, WINDOW_W,
                                          SDL_WINDOW_SHOWN);

    SDL_Surface *icon = SDL_LoadBMP("bitmaps/icon.bmp");
    SDL_SetWindowIcon(window, icon);
    SDL_FreeSurface(icon);

    std::cout << "Window creation successful\n";

    if(window == NULL){
        std::cout << "Window failed\n";
        return EXIT_FAILURE;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1,
                                                SDL_RENDERER_ACCELERATED |
                                                SDL_RENDERER_PRESENTVSYNC);

    if(renderer == NULL){
        SDL_DestroyWindow(window);
        std::cout << "Renderer creation failed\n";
        return EXIT_FAILURE;
    }

    std::cout << "Renderer creation successful\n";

    //SDL_SetRenderDrawColor(renderer, 24, 24, 24, 255);
    //SDL_RenderClear(renderer);

    renderField(renderer);

    SDL_Event e;

    int quit = 0;
    int windowsc = 0;

    while(!quit){
        while(SDL_PollEvent(&e)){
            switch(e.type){
            case SDL_QUIT:
                quit = 1;
                break;
            case SDL_MOUSEBUTTONDOWN:
                //std::cout << "Mouse\n";
                handleClick(e.button.x, e.button.y, (int)e.button.button, renderer);
                //std::cout << (int)e.button.button;
                break;
            default: {}
            }
        }

        //SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

        /*
        for(int i = 0; i < WINDOW_W; i += CELL_WIDTH){
            SDL_RenderDrawLine(renderer, i, 0, i, WINDOW_W);
            SDL_RenderDrawLine(renderer, 0, i, WINDOW_W, i);
        }
        */

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
