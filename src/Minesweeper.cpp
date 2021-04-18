#include "../include/Cell.hpp"
#include "../include/Minesweeper.hpp"
#include <ctime>
#include <random>

Minesweeper::Minesweeper(const int width, const int height, const int difficulty)
    : m_width { width },
      m_height { height },
      m_difficulty { difficulty },
      m_generator { std::mt19937(time(nullptr)) }
{
    char* base_path = SDL_GetBasePath();
    m_executable_path = base_path;
    SDL_free(base_path);

    cells.resize(m_width * m_height);
    generate_cells();
}

Minesweeper::~Minesweeper(){
    SDL_DestroyTexture(Cell::get_texture());
}

void Minesweeper::assign_renderer(SDL_Renderer*& renderer){
    m_renderer = renderer;
}

int Minesweeper::get_window_width() const {
    return m_width * Cell::width;
}

int Minesweeper::get_window_height() const {
    return m_height * Cell::width;
}

bool Minesweeper::valid_coordinates(const int x, const int y) const {
    return y >= 0 && x >= 0 && y < m_height && x < m_width;
}

int Minesweeper::get_nearby_mines(const int x, const int y) const {
    int nearby = 0;
    for(int i = y - 1; i <= y + 1; ++i){
        for(int j = x - 1; j <= x + 1; ++j){
            if(!(i == y && j == x) && valid_coordinates(j, i)){
                if(cells[i * m_width + j].type() == Cell::Type::Mine)
                    nearby++;
            }
        }
    }
    return nearby;
}

void Minesweeper::initialize_texture(){
    SDL_Surface *surface = SDL_LoadBMP((m_executable_path + std::string("bitmaps/cells.bmp")).c_str());

    if(surface == nullptr){
        fprintf(stderr, "Failed to load bitmap: %s\n", SDL_GetError());
        return;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(m_renderer, surface);

    if(texture == nullptr){
        fprintf(stderr, "Failed to create texture: %s\n", SDL_GetError());
        return;
    }

    Cell::set_texture(texture);
    SDL_FreeSurface(surface);
}

void Minesweeper::reveal_nearby_empty(const int x, const int y){
    for(int ry = y - 1; ry <= y + 1; ++ry){
        for(int rx = x - 1; rx <= x + 1; ++rx){
            Cell &current_cell = cells[ry * m_width + rx];
            if(!(ry == y && rx == x) && valid_coordinates(rx, ry)){
                if(current_cell.type() != Cell::Type::Mine && !current_cell.revealed()){
                    if(!current_cell.flagged()){
                        current_cell.reveal();
                        current_cell.render(m_renderer);
                    }
                    if(current_cell.type() == Cell::Type::Nearby0)
                        reveal_nearby_empty(rx, ry);
                }
            }
        }
    }
}

void Minesweeper::render_hidden_field(){
    for(Cell &cell : cells){
        cell.render(m_renderer, Cell::Type::Hidden);
    }
}

void Minesweeper::reveal_all_cells(){
    for(Cell &cell : cells){
        if(cell.type() == Cell::Type::Mine && cell.flagged()){
            cell.render(m_renderer, Cell::Type::Flag);
            continue;
        }
        if(cell.type() != Cell::Type::Mine && cell.flagged()){
            cell.render(m_renderer, Cell::Type::FalseMine);
            continue;
        }
        cell.render(m_renderer);
    }
}

bool Minesweeper::check_win() const {
    for(Cell cell : cells){
        if((cell.type() == Cell::Type::Mine && !cell.flagged()) || (cell.type() != Cell::Type::Mine && cell.flagged()))
            return false;
    }
    return true;
}

void Minesweeper::generate_cells(){
    m_number_of_mines = 0;
    for(int y = 0; y < m_height; ++y){
        for(int x = 0; x < m_width; ++x){
            int pos = y * m_width + x;
            Cell::Type cell_type = Cell::Type::Nearby0;
            if(m_generator() % m_difficulty == 0){
                cell_type = Cell::Type::Mine;
                m_number_of_mines++;
            }
            cells[pos] = Cell(x, y, cell_type);
        }
    }

    for(int y = 0; y < m_height; ++y){
        for(int x = 0; x < m_width; ++x){
            int pos = y * m_width + x;
            if(cells[pos].type() != Cell::Type::Mine){
                int nearby = get_nearby_mines(x, y);
                cells[pos].set_type(Cell::nearby_mines_to_cell_type(nearby));
            }
        }
    }
}

void Minesweeper::mouse_up_event(const SDL_Event& event){
    if(m_game_over){
        m_game_over = false;
        return;
    }

    int x = event.button.x / Cell::width;
    int y = event.button.y / Cell::width;

    int pos = y * m_width + x;

    Cell &clicked_cell = cells[m_current_cell_pos];

    if(m_current_cell_pos != pos){
        if(!clicked_cell.revealed())
            clicked_cell.render(m_renderer, Cell::Type::Hidden);
        return;
    }

    Cell &current_cell = cells[pos];

    switch(event.button.button){
    case SDL_BUTTON_LEFT:
        // Very rare case (well, depends on difficulty and size of grid), when the RNG gods blessed you and gave you 0 mines
        if(m_number_of_mines == 0){
            m_game_over = true;
        }
        if(!current_cell.flagged()){
            if(current_cell.type() == Cell::Type::Mine){
                reveal_all_cells();
                current_cell.render(m_renderer, Cell::Type::TriggeredMine);
                m_game_over = true;
                return;
            }
            if(current_cell.type() == Cell::Type::Nearby0 && !current_cell.revealed()){
                current_cell.reveal();
                current_cell.render(m_renderer);
                reveal_nearby_empty(x, y);
                return;
            }
            if(!current_cell.revealed()){
                current_cell.reveal();
                current_cell.render(m_renderer);
            }
        }
        break;
    case SDL_BUTTON_RIGHT:
        if(!current_cell.revealed()){
            current_cell.toggle_flag();
            current_cell.render(m_renderer, current_cell.flagged() ? Cell::Type::Flag : Cell::Type::Hidden);
            if(check_win()){
                reveal_all_cells();
                m_game_over = true;
            }
        }
        break;
    default: {}
    }
}

void Minesweeper::mouse_down_event(const SDL_Event& event){
    if(m_game_over){
        generate_cells();
        render_hidden_field();
        return;
    }

    int x = event.button.x / Cell::width;
    int y = event.button.y / Cell::width;
    m_current_cell_pos = y * m_width + x;

    Cell &current_cell = cells[m_current_cell_pos];

    switch(event.button.button){
    case SDL_BUTTON_LEFT:
    case SDL_BUTTON_RIGHT:
        if(!current_cell.revealed() && !current_cell.flagged())
            current_cell.render(m_renderer, Cell::Type::Nearby0);
        break;
    default: {}
    }
}
