#include "../include/Cell.hpp"
#include <SDL2/SDL.h>

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

bool Cell::flagged() const {
    return m_flagged;
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

void Cell::render(SDL_Renderer* renderer, Cell::Type type){
    SDL_Rect texture_read_rect, texture_write_rect;

    texture_read_rect.x = 0;
    texture_read_rect.y = Cell::width * static_cast<size_t>(type);
    texture_read_rect.w = Cell::width;
    texture_read_rect.h = Cell::width;

    texture_write_rect.x = m_x * Cell::width;
    texture_write_rect.y = m_y * Cell::width;
    texture_write_rect.w = Cell::width;
    texture_write_rect.h = Cell::width;

    int render_copy_result = SDL_RenderCopy(renderer, Cell::m_texture, &texture_read_rect, &texture_write_rect);
    if(render_copy_result < 0){
        fprintf(stderr, "Failed to render texture: %s\n", SDL_GetError());
    }
}

void Cell::render(SDL_Renderer* renderer){
    render(renderer, m_type);
}
