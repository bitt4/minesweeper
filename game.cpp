#include <SDL2/SDL.h>
#include <ctime>
#include <cstdlib>

#include "include/consts.hpp"
#include "include/game.hpp"
#include "include/render.hpp"

void board_t::init(){
    
    srand(time(NULL));
    int randomNumber;
    
    for(int i = 0; i < CELLS_X*CELLS_Y; i++){
        randomNumber = rand();
        if(randomNumber%5 == 0){
            field[i] = Mine;
        }
        else{
            field[i] = Clear;
        }
    }
}

void handleClick(SDL_Renderer *renderer, const board_t *board, int x, int y, int button){
    //std::cout << x/CELL_WIDTH << " " << y/CELL_WIDTH << "" << "\n";

    int id;

    switch(button){
    case 1:
        clickOnTile(renderer, board, x, y);
        break;
    case 3:
        drawImage(renderer, x, y, Flag); //still for testing
        break;
    default:
        return;
    }

    //drawImage(renderer, x, y, id);
}
