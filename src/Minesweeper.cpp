#include "../include/Minesweeper.hpp"
#include <random>
#include <ctime>
#include <cstdlib>

Minesweeper::Minesweeper(int width, int height, int difficulty){
    int mine_field_size = width * height;

    mines = new uint8_t[mine_field_size]();

    time_t seed = time(NULL);
    std::mt19937 generator(seed);
    for(int y = 0; y < height; ++y){
        for(int x = 0; x < width; ++x){
            int pos = y * height + x;
            if(generator() % difficulty == 0){ /* this difficulty thing will be fixed later */
                mines[pos] = static_cast<uint8_t>(CellType::Mine);
            }
        }
    }
}

Minesweeper::~Minesweeper(){
    delete[] mines;
}
