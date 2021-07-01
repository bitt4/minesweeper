#ifndef MINESWEEPER_HPP
#define MINESWEEPER_HPP

#include <SDL2/SDL.h>
#include "Cell.hpp"
#include <random>
#include <vector>

class Minesweeper {
private:
    const int m_width;
    const int m_height;
    const int m_difficulty;
    int m_current_cell_pos { 0 };
    int m_number_of_mines { 0 };
    int m_number_of_revealed_cells { 0 };
    int m_cells_flagged_correctly { 0 };
    bool m_game_over { false };
    std::string m_executable_path;
    std::mt19937 m_generator;
    SDL_Renderer* m_renderer;

    std::vector<Cell> cells;

    int get_nearby_mines(const int x, const int y) const;
    void render_cell(Cell&);
    void render_cell(Cell&, Cell::Type);
    void reveal_cell(Cell&);
    void reveal_nearby_empty(const int x, const int y);
    void reveal_all_cells();
    bool valid_coordinates(const int x, const int y) const;
    bool check_win() const;
    void generate_cells();
public:
    Minesweeper(const int width, const int height, const int difficulty = 7);
    ~Minesweeper();
    int get_window_width() const;
    int get_window_height() const;
    std::string get_executable_path() const { return m_executable_path; }
    void assign_renderer(SDL_Renderer*& renderer);
    void render_hidden_field();
    void initialize_texture();
    void mouse_down_event(const SDL_Event& event);
    void mouse_up_event(const SDL_Event& event);
};

#endif //MINESWEEPER_HPP
