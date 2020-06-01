void handleClick(int x, int y, int button, SDL_Renderer *renderer);

typedef struct {
    int field[CELLS_X * CELLS_Y];
    void init();
} board_t;