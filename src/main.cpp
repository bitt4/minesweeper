#include "../include/Minesweeper.hpp"
#include <SDL2/SDL.h>
#include <cstdio>

int main(){

    if(SDL_Init(SDL_INIT_VIDEO) != 0){
        fprintf(stderr, "SDL Initialization error: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    Minesweeper minesweeper(24, 24);

    SDL_Window *window = SDL_CreateWindow("Minesweeper",
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          minesweeper.get_window_width(),
                                          minesweeper.get_window_height(),
                                          SDL_WINDOW_SHOWN);

    if(window == nullptr){
        fprintf(stderr, "Window creation failed: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window,
                                                -1,
                                                SDL_RENDERER_ACCELERATED);

    if(renderer == nullptr){
        fprintf(stderr, "Renderer creation failed: %s\n", SDL_GetError());
    }

    minesweeper.assign_renderer(renderer);
    minesweeper.initialize_texture();
    minesweeper.render_hidden_field();
    SDL_RenderPresent(renderer);

    bool quit = false;
    SDL_Event e;

    while(!quit){
        while(SDL_PollEvent(&e)){
            switch(e.type){
            case SDL_QUIT:
                quit = true;
                break;
            case SDL_MOUSEBUTTONDOWN:
                minesweeper.mouse_down_event(e);
                break;
            default:
                break;
            }
        }
        // minesweeper.render(renderer);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
