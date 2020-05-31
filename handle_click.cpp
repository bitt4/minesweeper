#include <SDL2/SDL.h>

#include "include/handle_click.hpp"
#include "include/render.hpp"

void handleClick(int x, int y, int button, SDL_Renderer *renderer){
    //std::cout << x/CELL_WIDTH << " " << y/CELL_WIDTH << "" << "\n";

    int id;

    switch(button){
    case 1:
        id = 15;
        break;
    case 3:
        id = 1;
        break;
    default:
        return;
    }

    drawImage(renderer, x, y, id);
}
