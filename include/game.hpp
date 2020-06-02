typedef struct {
    int field[CELLS_X * CELLS_Y];
    void init();
} board_t;

void handleClick(SDL_Renderer *renderer, const board_t *board, int x, int y, int button);