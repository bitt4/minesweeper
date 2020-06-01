#include <SDL2/SDL.h>

#include "include/consts.hpp"
#include "include/game.hpp"
#include "include/render.hpp"

void board_t::init(){
    for(int i = 0; i < CELLS_X*CELLS_Y; i++){
        field[i] = Mine;
    }
}

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
