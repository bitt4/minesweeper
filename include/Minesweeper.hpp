#ifndef MINESWEEPER_HPP
#define MINESWEEPER_HPP

#include <SDL2/SDL.h>
#include "../include/Cell.hpp"
#include <vector>

class Minesweeper {
private:
    const int m_width;
    const int m_height;
    const int m_difficulty;

    std::vector<Cell> cells;

    int get_nearby_mines(const int x, const int y) const;
    void set_flag(const int x, const int y, bool flagged);
    void draw_cell(SDL_Renderer* renderer, const int x, const int y);
    void reveal_nearby_empty(SDL_Renderer* renderer, const int x, const int y);
public:
    Minesweeper(const int width, const int height, const int difficulty = 7);
    ~Minesweeper();
    int get_window_width() const;
    int get_window_height() const;
    void render_hidden_field(SDL_Renderer* renderer);
    void initialize_texture(SDL_Renderer* renderer);
    void mouse_down_event(const SDL_Event& event);
};

#endif //MINESWEEPER_HPP
