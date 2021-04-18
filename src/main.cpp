#include "../include/Minesweeper.hpp"
#include <SDL2/SDL.h>
#include <cstdio>
#include <getopt.h>
#include <string>

void help();
int parse_option(const char*, int l, int r);

int main(int argc, char *argv[]){

    // default options for minesweeper initialization
    int width = 16;
    int height = 16;
    int difficulty = 7;

    static struct option long_options[] = {
        {"width", required_argument, NULL, 'w'},
        {"height", required_argument, NULL, 'h'},
        {"diff", required_argument, NULL, 'd'},
        {"help", no_argument, NULL, 'H'},
        {NULL, 0, NULL, 0}
    };

    int c;

    while ((c = getopt_long(argc, argv, "w:h:d:H", long_options, NULL)) != -1) {
        switch (c)
            {
            case 'w':
                width = parse_option(optarg, 1, 1000);    // Upper limit of 1000 cells is not really necessary
                break;
            case 'h':
                height = parse_option(optarg, 1, 1000);
                break;
            case 'd':
                difficulty = parse_option(optarg, 1, 10);
                break;
            case 'H':
                help();
                // lowercase 'h' is already used for height
                exit(1);
            case '?':
                fprintf(stderr, "%s: Use -H or --help to display options.\n", argv[0]);
                exit(1);
            default: {}
            }
    }

    Minesweeper minesweeper(width, height, difficulty);

    if(SDL_Init(SDL_INIT_VIDEO) != 0){
        fprintf(stderr, "SDL Initialization error: %s\n", SDL_GetError());
        return 1;
    }

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

    SDL_Surface *icon = SDL_LoadBMP((minesweeper.get_executable_path() + std::string("bitmaps/icon.bmp")).c_str());

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

    while(!quit && SDL_WaitEvent(&e)){
        switch(e.type){
        case SDL_QUIT:
            quit = true;
            break;
        case SDL_MOUSEBUTTONDOWN:
            minesweeper.mouse_down_event(e);
            break;
        case SDL_MOUSEBUTTONUP:
            minesweeper.mouse_up_event(e);
            break;
        default:
            break;
        }
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

void help(){
    fprintf(stderr, "Usage: minesweeper [OPTIONS]...\n"
            "\n"
            "Options:\n"
            "  -w, --width=[NUM]    width of board in cells\n"
            "  -h, --height=[NUM]   height of board in cells\n"
            "  -d, --diff=[NUM]     set difficulty, chance 1 in NUM that a mine will be generated\n"
            "  -H, --help           print this help message\n"
            "\n");
}

int parse_option(const char* option, int l, int r){
    long int value = strtol(option, nullptr, 0);
    if(!(l <= value && value <= r)){
        fprintf(stderr, "Invalid argument '%s'\n", option);
        exit(1);
    }
    return value;
}
