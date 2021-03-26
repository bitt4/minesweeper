#ifndef MINESWEEPER_HPP
#define MINESWEEPER_HPP

#include <SDL2/SDL.h>
#include "../include/Cell.hpp"

class Minesweeper {
private:
    const int m_width;
    const int m_height;
    const int m_difficulty;

    Cell* cells;
    SDL_Texture* cell_textures;

    int get_width() const;
    int get_height() const;
    int get_nearby_mines(const int x, const int y) const;
    void set_flag(const int x, const int y, bool flagged);
    void initialize_texture(SDL_Renderer* renderer);
    void draw_cell(SDL_Renderer* renderer, const int x, const int y);
    void reveal_nearby_empty(SDL_Renderer* renderer, const int x, const int y);
public:
    Minesweeper(const int width, const int height, const int difficulty);
    ~Minesweeper();
    void mouse_down_event(SDL_Event& event);
};

#endif //MINESWEEPER_HPP
