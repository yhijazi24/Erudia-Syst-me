#ifndef TRIOMINO_RENDERER_H
#define TRIOMINO_RENDERER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "triomino_types.h"
#include "triomino_game.h"

void dessiner_triomino(SDL_Renderer* renderer, TTF_Font* font, Triomino* triomino, int x, int y, int size, bool rotate);  

void render_triomino_plateau(Game* game, TriominoGame* triomino_state);
void render_triominos(Game* game);

#endif 
