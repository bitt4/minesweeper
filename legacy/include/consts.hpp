#ifndef _CONSTS_H
#define _CONSTS_H

#define CELL_WIDTH 16
#define CELLS_X 32	//number of cells in one row
#define CELLS_Y 24	//number of cells in one column

#define WINDOW_W CELLS_X*CELL_WIDTH
#define WINDOW_H CELLS_Y*CELL_WIDTH

enum FieldType {
    Hidden,
    Flag,
    Question,
    TriggeredMine,
    FalseMine,
    Mine,
    Question2,
    Clear = 15
};

enum GameState {
    Playing,
    GameOver
};

#endif // _CONSTS_H