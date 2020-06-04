#include <SDL2/SDL.h>
#include <ctime>
#include <cstdlib>

#include "include/consts.hpp"
#include "include/game.hpp"
#include "include/render.hpp"

bool flags[CELLS_X*CELLS_Y];

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

bool flagTile(int x, int y){
    if(flags[y * CELLS_X + x] == true){
        flags[y * CELLS_X + x] = false;
        return false;
    }
    if(flags[y * CELLS_X + x] == false){
        flags[y * CELLS_X + x] = true;
        return true;
    }
}

void handleClick(SDL_Renderer *renderer, const board_t *board, int x, int y, int button){
    //std::cout << x/CELL_WIDTH << " " << y/CELL_WIDTH << "" << "\n";

    int id;

    switch(button){
    case 1:
        clickOnTile(renderer, board, flags, x, y);
        break;
    case 3:
        drawImage(renderer, x, y, Flag); //still for testing
        flagTile(x, y);
        break;
    default:
        return;
    }

    //drawImage(renderer, x, y, id);
}
