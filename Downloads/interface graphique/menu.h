#ifndef MENU_H
#define MENU_H

#include "game.h"
#include <SDL2/SDL.h>
#include <stdbool.h>


bool menu_init(Game* game);


void menu_cleanup(Game* game);


void handle_menu_events(Game* game, SDL_Event* event);


void render_menu(Game* game);

#endif 
