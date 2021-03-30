#include "../include/Cell.hpp"
#include "../include/Minesweeper.hpp"
#include <ctime>
#include <random>

Minesweeper::Minesweeper(const int width, const int height, const int difficulty)
    : m_width { width },
      m_height { height },
      m_difficulty { difficulty }
{
    size_t cell_field_size = m_width * m_height;
    cells.resize(cell_field_size);

    time_t seed = time(NULL);
    std::mt19937 generator(seed);
    for(Cell &cell : cells){
        if(generator() % difficulty == 0)
            cell.set_type(Cell::Type::Mine);
    }

    for(int y = 0; y < m_height; ++y){
        for(int x = 0; x < m_width; ++x){
            int pos = y * m_height + x;
            if(cells[pos].type() != Cell::Type::Mine){
                int nearby = get_nearby_mines(x, y);
                cells[pos].set_type(static_cast<Cell::Type>(15 - nearby)); // TODO: maybe add helper funciton for this?
            }
        }
    }
}

Minesweeper::~Minesweeper(){
    SDL_DestroyTexture(Cell::get_texture());
}

int Minesweeper::get_nearby_mines(const int x, const int y) const {
    int nearby = 0;
    for(int i = y - 1; i <= y + 1; ++i){
        for(int j = x - 1; j <= x + 1; ++j){
            if(i != 0 && j != 0 && i >= 0 && j >= 0 && i < m_height && j < m_width){
                if(cells[i * m_height + j].type() == Cell::Type::Mine)
                    nearby++;
            }
        }
    }
    return nearby;
}

void Minesweeper::initialize_texture(SDL_Renderer* renderer){
    SDL_Surface *surface = SDL_LoadBMP((SDL_GetBasePath() + std::string("../bitmaps/cells.bmp")).c_str());
    Cell::set_texture(SDL_CreateTextureFromSurface(renderer, surface));
    SDL_FreeSurface(surface);
}

// void Minesweeper::draw_cell(SDL_Renderer* renderer, const int x, const int y){
//     SDL_Rect texture_read_rect, texture_write_rect;
//     int pos = y * m_height + x;

//     texture_read_rect.x = 0;
//     texture_read_rect.y = Cell::width * static_cast<size_t>(cells[pos].type());
//     texture_read_rect.w = Cell::width;
//     texture_read_rect.h = Cell::width;

//     texture_write_rect.x = x * Cell::width;
//     texture_write_rect.y = y * Cell::width;
//     texture_write_rect.w = Cell::width;
//     texture_write_rect.h = Cell::width;

//     SDL_RenderCopy(renderer, cell_textures, &texture_read_rect, &texture_write_rect);
// }

void Minesweeper::reveal_nearby_empty(SDL_Renderer* renderer, const int x, const int y){
    // int pos = y * m_height + x;

    // TODO: make this look simpler
    for(int ry = y - 1; ry <= y + 1; ++ry){
        for(int rx = x - 1; rx <= x + 1; ++rx){
            Cell &current_cell = cells[ry * m_height + rx];
            if(current_cell.type() != Cell::Type::Mine && !current_cell.revealed()){
                current_cell.reveal();
                // current_cell.render(renderer);
                if(ry != 0 && rx != 0 && rx >= 0 && rx < m_width && ry >= 0 && ry < m_height)
                    reveal_nearby_empty(renderer, rx, ry);
            }
        }
    }
}

void Minesweeper::mouse_down_event(const SDL_Event& event){
    int x = event.button.x / Cell::width;
    int y = event.button.y / Cell::width;

    Cell &current_cell = cells[y * m_height + x];

    switch(event.button.button){
    case SDL_BUTTON_LEFT:
        if(current_cell.type() == Cell::Type::Mine){
            // reveal_all_cells();
            // current_cell.render(renderer, Cell::Type::TriggeredMine);
            // game_over();
            return;
        }
        if(!current_cell.revealed()){
            current_cell.reveal();
            // current_cell.render(renderer);
        }
        break;
    case SDL_BUTTON_RIGHT:
        current_cell.toggle_flag();
        // current_cell.render(renderer);
        // check_win();
        break;
    default: {}
    }
}
