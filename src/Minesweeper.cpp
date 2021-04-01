#include "../include/Cell.hpp"
#include "../include/Minesweeper.hpp"
#include <ctime>
#include <random>

Minesweeper::Minesweeper(const int width, const int height, const int difficulty)
    : m_width { width },
      m_height { height },
      m_difficulty { difficulty }
{
    time_t seed = time(NULL);
    std::mt19937 generator(seed);

    for(int y = 0; y < m_height; ++y){
        for(int x = 0; x < m_width; ++x){
            Cell::Type cell_type = Cell::Type::Nearby0;
            if(generator() % difficulty == 0)
                cell_type = Cell::Type::Mine;
            cells.push_back(Cell(x, y, cell_type));
        }
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

void Minesweeper::assign_renderer(SDL_Renderer *renderer){
    m_renderer = renderer;
}

int Minesweeper::get_window_width() const {
    return m_width * Cell::width;
}

int Minesweeper::get_window_height() const {
    return m_height * Cell::width;
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

void Minesweeper::initialize_texture(){
    SDL_Surface *surface = SDL_LoadBMP((SDL_GetBasePath() + std::string("bitmaps/cells.bmp")).c_str());
    Cell::set_texture(SDL_CreateTextureFromSurface(m_renderer, surface));
    SDL_FreeSurface(surface);
}

void Minesweeper::reveal_nearby_empty(const int x, const int y){
    // int pos = y * m_height + x;

    // TODO: make this look simpler
    for(int ry = y - 1; ry <= y + 1; ++ry){
        for(int rx = x - 1; rx <= x + 1; ++rx){
            Cell &current_cell = cells[ry * m_height + rx];
            if(current_cell.type() != Cell::Type::Mine && !current_cell.revealed()){
                current_cell.reveal();
                // current_cell.render(m_renderer);
                if(ry != 0 && rx != 0 && rx >= 0 && rx < m_width && ry >= 0 && ry < m_height)
                    reveal_nearby_empty(rx, ry);
            }
        }
    }
}

void Minesweeper::render_hidden_field(){
    for(Cell &cell : cells){
        cell.render(m_renderer, Cell::Type::Hidden);
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
            // current_cell.render(m_renderer, Cell::Type::TriggeredMine);
            // game_over();
            return;
        }
        if(!current_cell.revealed()){
            current_cell.reveal();
            // current_cell.render(m_renderer);
        }
        break;
    case SDL_BUTTON_RIGHT:
        current_cell.toggle_flag();
        // current_cell.render(m_renderer);
        // check_win();
        break;
    default: {}
    }
}
