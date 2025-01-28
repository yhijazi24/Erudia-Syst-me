#ifndef DOMINOS_TYPES_H
#define DOMINOS_TYPES_H

#include <SDL2/SDL.h>
#include <stdbool.h>
#include "game.h"

#define MAX_PLACEMENT_LOCATIONS 8
#define DOMINO_WIDTH 60
#define DOMINO_HEIGHT 120
#define PADDING 10

typedef struct {
    int left;
    int right;
    SDL_Rect rect;
    bool visible;
    bool selected;
    int rotation_angle; 
} Domino;


typedef struct {
   int x;           
   int y;           
   Domino* domino;  
   bool vertical;   
} PlacedDomino;

typedef struct {
   SDL_Rect rect;
   SDL_Rect yes_button;
   SDL_Rect no_button;
   bool is_visible;
} QuitDialog;

typedef struct {
    SDL_Rect location;
    bool is_valid;
} PlacementLocation;

typedef struct {
   Domino* player_hand;
   Domino* opponent_hand;
   Domino* deck;
   int deck_size;
   int hand_size;
   int board_size;
   int current_player;
   bool game_started;
   QuitDialog quit_dialog;
   SDL_Rect playing_area;
   PlacedDomino* placed_dominos;
   int placed_dominos_count;
   int max_placed_dominos;
   PlacementLocation placement_locations[MAX_PLACEMENT_LOCATIONS];
    int placement_location_count;
} DominosGame;

#endif