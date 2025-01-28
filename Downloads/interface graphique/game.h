#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <stdbool.h>

typedef enum {
    GAME_STATE_MENU,
    CONFIG_JOUEURS,
    GAME_STATE_DOMINOS,
    GAME_STATE_TRIOMINOS,
    GAME_STATE_QUIT
} GameState;

typedef enum {
    GAME_MODE_NONE,
    GAME_MODE_DOMINOS,
    GAME_MODE_TRIOMINOS
} GameMode;

typedef struct {
    SDL_Window* window;
    SDL_Renderer* renderer;
    int window_width;
    int window_height;
    GameState state;
    GameMode game_mode;
    bool running;
} Game;




bool game_init(Game* game);
void game_run(Game* game);
void game_cleanup(Game* game);

#endif 
