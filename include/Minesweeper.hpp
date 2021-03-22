#include <SDL2/SDL.h>

class Minesweeper {
private:
    enum class Mine {
        Hidden,
        Flag,
        Question,
        TriggeredMine,
        FalseMine,
        Mine,
        Question2,
        Clear = 15
    };
    int width;
    int height;
    static const cell_width { 16 };

    uint8_t* mines;

    int get_width();
    int get_height();
    void draw_cell(int x, int y);
    int get_nearby_cells(int x, int y);
    void reveal_nearby_empty(int x, int y);
public:
    Minesweeper(int width, int height, int difficulty);
    void render(SDL_Renderer *renderer);
    void click(int x, int y);
};
