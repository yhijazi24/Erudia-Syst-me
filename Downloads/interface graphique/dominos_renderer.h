#ifndef DOMINOS_RENDERER_H
#define DOMINOS_RENDERER_H

#include "game.h"
#include <stdbool.h>

bool init_dominos_renderer(Game* game);
void cleanup_dominos_renderer(Game* game);

void render_dominos(Game* game, int num_players);
void render_player_info(Game* game, int player, int x, int y, int num_players);
#endif 