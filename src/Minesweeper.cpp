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

void Minesweeper::initialize_texture() const {
    SDL_Surface *surface = SDL_LoadBMP((m_executable_path + "bitmaps/cells.bmp").c_str());

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

void Minesweeper::render_cell(const Cell& cell, Cell::Type type) const {
    SDL_Rect texture_read_rect, texture_write_rect;

    texture_read_rect.x = 0;
    texture_read_rect.y = Cell::width * static_cast<size_t>(type);
    texture_read_rect.w = Cell::width;
    texture_read_rect.h = Cell::width;

    texture_write_rect.x = cell.x() * Cell::width;
    texture_write_rect.y = cell.y() * Cell::width;
    texture_write_rect.w = Cell::width;
    texture_write_rect.h = Cell::width;

    int render_copy_result = SDL_RenderCopy(m_renderer, Cell::get_texture(), &texture_read_rect, &texture_write_rect);
    if(render_copy_result < 0){
        fprintf(stderr, "Failed to render texture: %s\n", SDL_GetError());
    }
}

void Minesweeper::render_cell(const Cell& cell) const {
    render_cell(cell, cell.type());
}

void Minesweeper::reveal_cell(Cell& cell){
    cell.reveal();
    m_number_of_revealed_cells++;
}

void Minesweeper::reveal_nearby_empty(const int x, const int y){
    for(int ry = y - 1; ry <= y + 1; ++ry){
        for(int rx = x - 1; rx <= x + 1; ++rx){
            if(valid_coordinates(rx, ry)){
                Cell &current_cell = cells[ry * m_width + rx];
                if(!(ry == y && rx == x)){
                    if(current_cell.type() != Cell::Type::Mine && !current_cell.revealed()){
                        if(!current_cell.flagged()){
                            reveal_cell(current_cell);
                            render_cell(current_cell);
                        }
                        if(current_cell.type() == Cell::Type::Nearby0)
                            reveal_nearby_empty(rx, ry);
                    }
                }
            }
        }
    }
}

void Minesweeper::render_hidden_field() const {
    for(const Cell &cell : cells){
        render_cell(cell, Cell::Type::Hidden);
    }
}

void Minesweeper::reveal_all_cells(){
    for(Cell &cell : cells){
        if(cell.flagged()){
            if(cell.type() == Cell::Type::Mine)
                render_cell(cell, Cell::Type::Flag);
            else
                render_cell(cell, Cell::Type::FalseMine);
            continue;
        }
        render_cell(cell);
    }
}

bool Minesweeper::check_win() const {
    int number_of_cells = m_width * m_height;
    if(m_number_of_revealed_cells == number_of_cells - m_number_of_mines){
        return true;
    }

    if(m_cells_flagged_correctly == m_number_of_mines){
        return true;
    }

    return false;
}

void Minesweeper::generate_cells(){
    m_number_of_mines = 0;
    m_number_of_revealed_cells = 0;
    m_cells_flagged_correctly = 0;

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
            render_cell(clicked_cell, Cell::Type::Hidden);
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
                // TODO: Reveal only mines, not all cells
                render_cell(current_cell, Cell::Type::TriggeredMine);
                m_game_over = true;
                break;
            }
            if(current_cell.type() == Cell::Type::Nearby0 && !current_cell.revealed()){
                reveal_cell(current_cell);
                render_cell(current_cell);
                reveal_nearby_empty(x, y);
                break;
            }
            if(!current_cell.revealed()){
                reveal_cell(current_cell);
                render_cell(current_cell);
            }
        }
        break;
    case SDL_BUTTON_RIGHT:
        if(!current_cell.revealed()){
            current_cell.toggle_flag();
            render_cell(current_cell, current_cell.flagged() ? Cell::Type::Flag : Cell::Type::Hidden);

            if(current_cell.type() == Cell::Type::Mine){
                if(current_cell.flagged())
                    m_cells_flagged_correctly++;
                else
                    m_cells_flagged_correctly--;
            }
            else {
                if(current_cell.flagged())
                    m_cells_flagged_correctly--;
                else
                    m_cells_flagged_correctly++;
            }
        }
        break;
    default: {}
    }

    if(check_win()){
        reveal_all_cells();
        // TODO: Render unchecked mines with flags instead of mine
        m_game_over = true;
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
            render_cell(current_cell, Cell::Type::Nearby0);
        break;
    default: {}
    }
}
