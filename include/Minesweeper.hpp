#ifndef MINESWEEPER_HPP
#define MINESWEEPER_HPP
#include <SDL2/SDL.h>

class Minesweeper {
private:
    enum CellType {
        Hidden,
        Flag,
        Question,
        TriggeredMine,
        FalseMine,
        Mine,
        Question2,
        Clear = 15
    };
    const int m_width;
    const int m_height;
    const int m_difficulty;
    static const size_t cell_width { 16 };

    uint8_t* mines;
    SDL_Texture* cell_textures;

    int get_width() const;
    int get_height() const;
    void draw_cell(SDL_Renderer* renderer, const int x, const int y);
    int get_nearby_mines(const int x, const int y) const;
    void set_flag(const int x, const int y, bool flagged);
    void reveal_nearby_empty(const int x, const int y) const;
    void initialize_texture(SDL_Renderer* renderer);
public:
    Minesweeper(const int width, const int height, const int difficulty);
    ~Minesweeper();
    void render(SDL_Renderer *renderer);
    void mouse_down_event(SDL_Event *event);
};

#endif //MINESWEEPER_HPP
