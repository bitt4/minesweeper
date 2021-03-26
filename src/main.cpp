#include <SDL2/SDL.h>
#include <cstdio>

int main(){

    if(SDL_Init(SDL_INIT_VIDEO) != 0){
        fprintf(stderr, "SDL Initialization error.\n");
        return EXIT_FAILURE;
    }

    SDL_Window *window = SDL_CreateWindow("Minesweeper",
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          640/*minesweeper.get_width()*/,
                                          640/*minesweeper.get_height()*/,
                                          SDL_WINDOW_SHOWN);

    if(window == NULL){
        fprintf(stderr, "Window creation failed.\n");
        return EXIT_FAILURE;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window,
                                                -1,
                                                SDL_RENDERER_ACCELERATED);

    if(renderer == NULL){
        fprintf(stderr, "Renderer creation failed.\n");
    }

    bool quit = false;
    SDL_Event e;

    while(!quit){
        while(SDL_PollEvent(&e)){
            switch(e.type){
            case SDL_QUIT:
                quit = true;
                break;
            case SDL_MOUSEBUTTONDOWN:
                // minesweeper.mouse_down_event(event);
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
