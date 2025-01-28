#ifndef DOMINOS_EVENTS_H
#define DOMINOS_EVENTS_H

#include <SDL2/SDL.h>
#include "game.h"
#include "dominos_types.h"

void handle_dominos_events(Game* game, SDL_Event* event);

#endif