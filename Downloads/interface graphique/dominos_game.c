#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "dominos_game.h"
#include "dominos_types.h"
#include "dominos_renderer.h"

#define DOMINO_WIDTH 60
#define DOMINO_HEIGHT 120
#define PADDING 10

static DominosGame dominos_state = {0};

void initialize_domino_deck(DominosGame* game) {
    int deck_index = 0;

    for (int left = 0; left <= 6; left++) {
        for (int right = left; right <= 6; right++) {
            game->deck[deck_index].left = left;
            game->deck[deck_index].right = right;
            game->deck[deck_index].visible = true;
            game->deck[deck_index].selected = false;
            deck_index++;
        }
    }

    game->deck_size = 28;
}

void select_player_hand(Game* game, DominosGame* dominos_game, Domino* hand, bool is_visible, int player, int num_players) {
    int dominoes_per_player = (num_players >= 3) ? 6 : 7;

    int x_position = 0;
    int y_position = 0;
    bool rotate = false; 
    int rotation_angle = 0; 
    int spacing = 10; 

    
    if (player == 1) { 
        x_position = (game->window_width - (DOMINO_WIDTH + spacing) * dominoes_per_player + spacing) / 2; 
        y_position = game->window_height - DOMINO_HEIGHT - 10; 
    } else if (player == 2) { 
        x_position = (game->window_width - (DOMINO_WIDTH + spacing) * dominoes_per_player + spacing) / 2; 
        y_position = 10; 
    } else if (player == 3) { 
        x_position = game->window_width - DOMINO_HEIGHT - 50;
        y_position = (game->window_height - (DOMINO_WIDTH + spacing) * dominoes_per_player + spacing) / 2 - 150; 
        rotate = true;     
        rotation_angle = 90;
    } else if (player == 4) { 
        x_position = 50;
        y_position = (game->window_height - (DOMINO_WIDTH + spacing) * dominoes_per_player + spacing) / 2 - 150; 
        rotate = true;     
        rotation_angle = -90;
    }

    spacing = rotate ? DOMINO_HEIGHT + 10 : DOMINO_WIDTH + 10; 

    for (int i = 0; i < dominoes_per_player; i++) {
        hand[i] = dominos_game->deck[i];
        hand[i].selected = false;
        hand[i].visible = is_visible;

        if (rotate) {
            hand[i].rect = (SDL_Rect){
                .x = x_position,
                .y = y_position + i * spacing,
                .w = DOMINO_HEIGHT,
                .h = DOMINO_WIDTH
            };
        } else {
            hand[i].rect = (SDL_Rect){
                .x = x_position + i * spacing,
                .y = y_position,
                .w = DOMINO_WIDTH,
                .h = DOMINO_HEIGHT
            };
        }
    }

    
    memmove(dominos_game->deck, dominos_game->deck + dominoes_per_player, 
            sizeof(Domino) * (dominos_game->deck_size - dominoes_per_player));
    dominos_game->deck_size -= dominoes_per_player;
}





bool dominos_init(Game* game, int num_players) {
    dominos_state.hand_size = (num_players >= 3) ? 6 : 7;
    dominos_state.placement_location_count = 0;

    dominos_state.player_hand = malloc(sizeof(Domino) * dominos_state.hand_size);
    dominos_state.opponent_hand = malloc(sizeof(Domino) * dominos_state.hand_size * (num_players - 1));
    dominos_state.deck = malloc(sizeof(Domino) * 28);

    if (!dominos_state.player_hand || !dominos_state.opponent_hand || !dominos_state.deck) {
        SDL_Log("Memory allocation failed during initialization.");
        return false;
    }

    initialize_domino_deck(&dominos_state);
    select_player_hand(game, &dominos_state, dominos_state.player_hand, true, 1, num_players);
    for (int i = 2; i <= num_players; i++) {
        select_player_hand(game, &dominos_state, dominos_state.opponent_hand + (i - 2) * dominos_state.hand_size, false, i, num_players);
    }

    dominos_state.playing_area = (SDL_Rect){
        .x = (game->window_width - 800) / 2,
        .y = (game->window_height - 500) / 2,
        .w = 800,
        .h = 500
    };

    if (!init_dominos_renderer(game)) {
        SDL_Log("Dominos renderer initialization failed.");
        return false;
    }

    return true;
}




void dominos_cleanup(Game* game) {
    if (dominos_state.player_hand) {
        free(dominos_state.player_hand);
        dominos_state.player_hand = NULL;
    }
    if (dominos_state.opponent_hand) {
        free(dominos_state.opponent_hand);
        dominos_state.opponent_hand = NULL;
    }
    if (dominos_state.deck) {
        free(dominos_state.deck);
        dominos_state.deck = NULL;
    }
    cleanup_dominos_renderer(game);
}

DominosGame* get_dominos_game_state(void) {
    return &dominos_state;
}

void calculate_placement_locations(DominosGame* game, Domino* selected_domino, PlacementLocation* locations, int* location_count) {
    *location_count = 0;

    
    if (game->placed_dominos_count == 0) {
        SDL_Rect center_location = {
            game->playing_area.x + (game->playing_area.w - selected_domino->rect.w) / 2,
            game->playing_area.y + (game->playing_area.h - selected_domino->rect.h) / 2,
            selected_domino->rect.w,
            selected_domino->rect.h
        };
        locations[0].location = center_location;
        locations[0].is_valid = true;
        *location_count = 1;
        return;
    }
}