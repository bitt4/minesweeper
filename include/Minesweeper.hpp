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
    const int width;
    const int height;
    const int difficulty;
    static const size_t cell_width { 16 };

    uint8_t* mines;
    SDL_Texture* cell_textures;

    int get_width();
    int get_height();
    void draw_cell(SDL_Renderer* renderer, int x, int y);
    int get_nearby_mines(int x, int y);
    void set_flag(int x, int y, bool flagged);
    void reveal_nearby_empty(int x, int y);
    void initialize_texture(SDL_Renderer* renderer);
public:
    Minesweeper(int width, int height, int difficulty);
    ~Minesweeper();
    void render(SDL_Renderer *renderer);
    void mouse_down_event(SDL_Event *event);
};

#endif //MINESWEEPER_HPP
