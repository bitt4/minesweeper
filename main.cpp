#include <SDL2/SDL.h>

#include <iostream>
//#include <cstdio>
#include <cstdlib>

//#define DEBUG

//#define WINDOW_H 401
#include "include/consts.hpp"
#include "include/game.hpp"
#include "include/render.hpp"

int main(int argc, char *argv[]){

    if(SDL_Init(SDL_INIT_EVERYTHING) != 0){
        std::cout << "Init failed\n";
        return EXIT_FAILURE;
    }

    std::cout << "Init successful\n";

    SDL_Window *window = SDL_CreateWindow("Minesweeper",
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          WINDOW_W, WINDOW_H,
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
    
    initRender(renderer);
    renderField(renderer);
    
    board_t board;
    board.init();

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
                handleClick(renderer, &board, e.button.x/CELL_WIDTH, e.button.y/CELL_WIDTH, (int)e.button.button);
                //std::cout << (int)e.button.button;
                break;
#ifdef DEBUG
            case SDL_KEYDOWN:
                revealField(renderer, &board);
                break;
#endif
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
