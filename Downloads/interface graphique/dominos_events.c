#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "dominos_events.h"
#include "dominos_types.h"
#include "dominos_game.h"

void handle_dominos_events(Game* game, SDL_Event* event) {
    int mouseX, mouseY;

    switch (event->type) {
        case SDL_KEYDOWN:
            if (event->key.keysym.sym == SDLK_ESCAPE) {
                DominosGame* dominos_state = get_dominos_game_state();
                if (!dominos_state->quit_dialog.is_visible) {
                    dominos_state->quit_dialog.is_visible = true;
                }
            }
            break;

        case SDL_MOUSEBUTTONDOWN:
            mouseX = event->button.x;
            mouseY = event->button.y;

            DominosGame* dominos_state = get_dominos_game_state();

            
            if (dominos_state->quit_dialog.is_visible) {
                if (mouseX >= dominos_state->quit_dialog.yes_button.x &&
                    mouseX <= dominos_state->quit_dialog.yes_button.x + dominos_state->quit_dialog.yes_button.w &&
                    mouseY >= dominos_state->quit_dialog.yes_button.y &&
                    mouseY <= dominos_state->quit_dialog.yes_button.y + dominos_state->quit_dialog.yes_button.h) {
                    game->state = GAME_STATE_MENU;
                    return;
                }

                if (mouseX >= dominos_state->quit_dialog.no_button.x &&
                    mouseX <= dominos_state->quit_dialog.no_button.x + dominos_state->quit_dialog.no_button.w &&
                    mouseY >= dominos_state->quit_dialog.no_button.y &&
                    mouseY <= dominos_state->quit_dialog.no_button.y + dominos_state->quit_dialog.no_button.h) {
                    dominos_state->quit_dialog.is_visible = false;
                    return;
                }
                return;
            }

            
            for (int i = 0; i < dominos_state->hand_size; i++) {
                SDL_Rect* rect = &dominos_state->player_hand[i].rect;
                if (mouseX >= rect->x && mouseX <= rect->x + rect->w &&
                    mouseY >= rect->y && mouseY <= rect->y + rect->h) {
                    
                    for (int j = 0; j < dominos_state->hand_size; j++) {
                        if (j != i) {
                            dominos_state->player_hand[j].selected = false;
                        }
                    }
                    
                    dominos_state->player_hand[i].selected = !dominos_state->player_hand[i].selected;

                    if (dominos_state->player_hand[i].selected) {
                        calculate_placement_locations(dominos_state,
                                                      &dominos_state->player_hand[i],
                                                      dominos_state->placement_locations,
                                                      &dominos_state->placement_location_count);
                    } else {
                        dominos_state->placement_location_count = 0;
                    }
                    break;
                }
            }
            break;
    }
}
