#include "../include/Cell.hpp"
#include <SDL2/SDL.h>
#include <cstdio>

SDL_Texture* Cell::m_texture { nullptr };

Cell::Cell(const int x, const int y, const Cell::Type type)
    : m_x { x },
      m_y { y },
      m_type { type }
{
}

void Cell::toggle_flag(){
    m_flagged = !m_flagged;
}

void Cell::set_type(Cell::Type type){
    m_type = type;
}

void Cell::reveal(){
    m_revealed = true;
}

void Cell::set_texture(SDL_Texture*& texture){
    m_texture = texture;
}

Cell::Type Cell::nearby_mines_to_cell_type(const int nearby) {
    return static_cast<Cell::Type>(15 - nearby);
    /* This works because of the way cell textures are organized in
     * bitmaps/cells.bmp
     */
}
