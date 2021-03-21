#ifndef _RENDER_H
#define _RENDER_H

void drawImage(SDL_Renderer *renderer, int x, int y, int id);
void initRender(SDL_Renderer *renderer);
void renderField(SDL_Renderer *renderer);
void revealField(SDL_Renderer *renderer, const board_t *board);
void clickOnTile(SDL_Renderer *renderer, board_t *board, bool *flags, int x, int y);
void renderFlag(SDL_Renderer *renderer, const bool *flags, int x, int y);
int revealNearby(SDL_Renderer *renderer, const board_t *board, int x, int y);
void revealGroupEmpty(SDL_Renderer *renderer, const board_t *board, int x, int y);
void revealEmpty(SDL_Renderer *renderer, const board_t *board);
void revealExceptMines(SDL_Renderer *renderer, const board_t *board);
void resetFlagsAndRevealed(SDL_Renderer *renderer, bool *flags);

#endif // _RENDER_H