#ifndef _GAME_H
#define _GAME_H

typedef struct {
    int field[CELLS_X * CELLS_Y];
    int state;
    void init();
} board_t;

void handleClick(SDL_Renderer *renderer, board_t *board, int x, int y, int button);
int getNearbyMines(const board_t *board, int x, int y);

#endif // _GAME_H