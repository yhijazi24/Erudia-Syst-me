#ifndef DOMINOS_H
#define DOMINOS_H

#include "game.h"
#include "dominos_types.h"
#include <SDL2/SDL.h>
#include <stdbool.h>


bool dominos_init(Game* game, int num_players);
void dominos_cleanup(Game* game);


void handle_dominos_events(Game* game, SDL_Event* event);
bool is_valid_move(DominosGame* game, Domino* domino, int position);
void play_domino(DominosGame* game, int player, int domino_index, int position);
bool is_valid_placement(DominosGame* game, Domino* domino, int x, int y, bool vertical);
void print_tiles(Domino* tiles, int size);
void calculate_placement_locations(DominosGame* game, Domino* selected_domino, PlacementLocation* locations, int* location_count);



DominosGame* get_dominos_game_state(void);
int get_domino_width(void);
int get_domino_height(void);
int get_padding(void);

#endif 