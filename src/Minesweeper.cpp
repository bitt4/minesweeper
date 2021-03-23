#include "../include/Minesweeper.hpp"
#include <random>
#include <ctime>
#include <cstdlib>

Minesweeper::Minesweeper(int width, int height, int difficulty)
    : width(width),
      height(height),
      difficulty(difficulty)
{
    int mine_field_size = width * height;
    mines = new uint8_t[mine_field_size]();

    time_t seed = time(NULL);
    std::mt19937 generator(seed);
    for(int y = 0; y < height; ++y){
        for(int x = 0; x < width; ++x){
            int pos = y * height + x;
            if(generator() % difficulty == 0){ /* this difficulty thing will be fixed later */
                mines[pos] = CellType::Mine;
            }
        }
    }
}

Minesweeper::~Minesweeper(){
    SDL_DestroyTexture(cell_textures);
    delete[] mines;
}

int Minesweeper::get_width(){
    return width;
}

int Minesweeper::get_height(){
    return height;
}

int Minesweeper::get_nearby_mines(int x, int y){
    int nearby = 0;
    for(int i = y-1; i <= y + 1; ++i){
        for(int j = x - 1; j <= x + 1; ++j){
            if(i != 0 && j != 0 && i >= 0 && j >= 0 && i < height && j < width){
                if(mines[y * height + x] == CellType::Mine)
                    nearby++;
            }
        }
    }
    return nearby;
}

void Minesweeper::set_flag(int x, int y, bool flagged = true){
    // we can use upper 4 bits for some additional info, since we use
    // only lower 4 bits for cell value (type of mine, etc...),

    // let's use most significant bit for setting flag on mine field
    int pos = y * height + x;
    if(flagged)
        mines[pos] |= (1 << 7);
    else
        mines[pos] &= ~(1 << 7);
}

void Minesweeper::initialize_texture(SDL_Renderer* renderer){
    SDL_Surface *surface = SDL_LoadBMP((SDL_GetBasePath() + std::string("../bitmaps/items.bmp")).c_str());
    cell_textures = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
}

void Minesweeper::draw_cell(SDL_Renderer* renderer, int x, int y){
    SDL_Rect texture_read_rect, texture_write_rect;
    int pos = y * height + x;

    texture_read_rect.x = 0;
    texture_read_rect.y = cell_width * mines[pos];
    texture_read_rect.w = cell_width;
    texture_read_rect.h = cell_width;

    texture_write_rect.x = x * cell_width;
    texture_write_rect.y = y * cell_width;
    texture_write_rect.w = cell_width;
    texture_write_rect.h = cell_width;

    SDL_RenderCopy(renderer, cell_textures, &texture_read_rect, &texture_write_rect);
}
