#include "../include/Cell.hpp"
#include <SDL2/SDL.h>

SDL_Texture* Cell::m_texture { nullptr };

Cell::Cell(const int x, const int y, const Cell::Type type = Cell::Type::Nearby0)
    : m_x { x },
      m_y { y },
      m_type { type }
{

}

void Cell::toggle_flag(){
    m_flagged = !m_flagged;
}

Cell::Type Cell::type() const {
    return m_type;
}

void Cell::set_type(Cell::Type type){
    m_type = type;
}

bool Cell::revealed() const {
    return m_revealed;
}

void Cell::reveal(){
    m_revealed = true;
}

void Cell::set_texture(SDL_Texture* texture){
    m_texture = texture;
}

SDL_Texture* Cell::get_texture(){
    return m_texture;
}
