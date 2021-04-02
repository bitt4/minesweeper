#include "../include/Minesweeper.hpp"
#include <SDL2/SDL.h>
#include <cstdio>
#include <string>

int main(){

    if(SDL_Init(SDL_INIT_VIDEO) != 0){
        fprintf(stderr, "SDL Initialization error: %s\n", SDL_GetError());
        return 1;
    }

    Minesweeper minesweeper(8, 8);

    SDL_Window *window = SDL_CreateWindow("Minesweeper",
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          minesweeper.get_window_width(),
                                          minesweeper.get_window_height(),
                                          SDL_WINDOW_SHOWN);

    if(window == nullptr){
        fprintf(stderr, "Window creation failed: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window,
                                                -1,
                                                SDL_RENDERER_ACCELERATED);

    if(renderer == nullptr){
        fprintf(stderr, "Renderer creation failed: %s\n", SDL_GetError());
    }

    char* base_path = SDL_GetBasePath();
    SDL_Surface *icon = SDL_LoadBMP((base_path + std::string("bitmaps/icon.bmp")).c_str());
    SDL_free(base_path);

    if(icon == nullptr){
        fprintf(stderr, "Could not load icon: %s\n", SDL_GetError());
        return 1;
    }

    SDL_SetWindowIcon(window, icon);
    SDL_FreeSurface(icon);

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
