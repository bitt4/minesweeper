#include "../include/Cell.hpp"
#include "../include/Minesweeper.hpp"
#include <ctime>
#include <random>

Cell::Type nearby_mines_to_cell_type(const int nearby);

Minesweeper::Minesweeper(const int width, const int height, const int difficulty)
    : m_width { width },
      m_height { height },
      m_difficulty { difficulty }
{
    time_t seed = time(nullptr);
    std::mt19937 generator(seed);

    cells.reserve(m_width * m_height);

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
                cells[pos].set_type(nearby_mines_to_cell_type(nearby));
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

bool Minesweeper::valid_coordinates(const int x, const int y) const {
    return y >= 0 && x >= 0 && y < m_height && x < m_width;
}

int Minesweeper::get_nearby_mines(const int x, const int y) const {
    int nearby = 0;
    for(int i = y - 1; i <= y + 1; ++i){
        for(int j = x - 1; j <= x + 1; ++j){
            if(!(i == y && j == x) && valid_coordinates(j, i)){
                if(cells[i * m_height + j].type() == Cell::Type::Mine)
                    nearby++;
            }
        }
    }
    return nearby;
}

Cell::Type nearby_mines_to_cell_type(const int nearby) {
    return static_cast<Cell::Type>(15 - nearby);
    /* This works because of the way cell textures are organized in
     * bitmaps/cells.bmp */
}

void Minesweeper::initialize_texture(){
    char* base_path = SDL_GetBasePath();
    SDL_Surface *surface = SDL_LoadBMP((base_path + std::string("bitmaps/cells.bmp")).c_str());

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
    SDL_free(base_path);
}

void Minesweeper::reveal_nearby_empty(const int x, const int y){
    // TODO: make this look simpler
    for(int ry = y - 1; ry <= y + 1; ++ry){
        for(int rx = x - 1; rx <= x + 1; ++rx){
            Cell &current_cell = cells[ry * m_height + rx];
            if(!(ry == y && rx == x) && valid_coordinates(rx, ry)){
                if(current_cell.type() != Cell::Type::Mine && !current_cell.revealed()){
                    current_cell.reveal();
                    current_cell.render(m_renderer);
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
    for(Cell &cell : cells)
        cell.render(m_renderer);
}

bool Minesweeper::check_win() const {
    for(Cell cell : cells){
        if((cell.type() == Cell::Type::Mine && !cell.flagged()) || (cell.type() != Cell::Type::Mine && cell.flagged()))
            return false;
    }
    return true;
}

void Minesweeper::mouse_down_event(const SDL_Event& event){
    int x = event.button.x / Cell::width;
    int y = event.button.y / Cell::width;

    Cell &current_cell = cells[y * m_height + x];

    switch(event.button.button){
    case SDL_BUTTON_LEFT:
        if(!current_cell.flagged()){
            if(current_cell.type() == Cell::Type::Mine){
                reveal_all_cells();
                current_cell.render(m_renderer, Cell::Type::TriggeredMine);
                // game_over();
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
                // restart();
            }
        }
        break;
    default: {}
    }
}
