#include <SDL2/SDL_ttf.h>
#include "dominos_types.h"
#include "dominos_game.h"
#include "dominos_renderer.h"
#include <stdio.h>

static TTF_Font* font = NULL;

bool init_dominos_renderer(Game* game) {
    font = TTF_OpenFont("assets/fonts/Arial.ttf", 24);
    if (!font) {
        SDL_Log("Failed to load font: %s", TTF_GetError());
        return false;
    }
    return true;
}
void cleanup_dominos_renderer(Game* game) {
    (void)game; 
    if (font) {
        TTF_CloseFont(font);
        font = NULL;
    }
}

void render_player_info(Game* game, int player, int x, int y, int num_players) {
    TTF_Font* font = TTF_OpenFont("assets/fonts/Arial.ttf", 24);
    if (font) {
        SDL_Color textColor = {255, 255, 255, 255}; 
        char player_info[50];
        snprintf(player_info, sizeof(player_info), "Player %d: %d", player, 0); 

        SDL_Surface* surface = TTF_RenderText_Solid(font, player_info, textColor);
        SDL_Texture* texture = SDL_CreateTextureFromSurface(game->renderer, surface);

        SDL_Rect text_rect;
        double angle = 0.0; 

        if (player == 1) { 
            text_rect = (SDL_Rect){
                .x = x - 180,
                .y = y + (DOMINO_HEIGHT / 2) - (surface->h / 2) - 10,
                .w = surface->w,
                .h = surface->h
            };
        } else if (player == 2) { 
            text_rect = (SDL_Rect){
                .x = x - 180,
                .y = y + (DOMINO_HEIGHT / 2) - (surface->h / 2) - 10,
                .w = surface->w,
                .h = surface->h
            };
        } else if (player == 3) { 
            text_rect = (SDL_Rect){
                .x = x + (DOMINO_HEIGHT / 2) - (surface->w / 2) - 30,
                .y = y + 150,
                .w = surface->w,
                .h = surface->h
            };
            angle = -90.0; 
        } else if (player == 4) { 
            text_rect = (SDL_Rect){
                .x = x + (DOMINO_HEIGHT / 2) - (surface->w / 2) - 80,
                .y = y + 150,
                .w = surface->w,
                .h = surface->h
            };
            angle = 90.0; 
        }

        
        SDL_RenderCopyEx(game->renderer, texture, NULL, &text_rect, angle, NULL, SDL_FLIP_NONE);

        SDL_FreeSurface(surface); 
        SDL_DestroyTexture(texture); 
        TTF_CloseFont(font); 
    }
}


void render_dominos(Game* game, int num_players) {
    DominosGame* dominos_state = get_dominos_game_state();

    SDL_SetRenderDrawColor(game->renderer, 34, 139, 34, 255); 
    SDL_RenderClear(game->renderer);

    SDL_SetRenderDrawColor(game->renderer, 169, 169, 169, 255); 
    SDL_RenderFillRect(game->renderer, &dominos_state->playing_area);

    for (int player = 1; player <= num_players; player++) {
        Domino* hand = (player == 1)
            ? dominos_state->player_hand
            : &dominos_state->opponent_hand[(player - 2) * dominos_state->hand_size];

        for (int i = 0; i < dominos_state->hand_size; i++) {
            SDL_Rect rect = hand[i].rect;

            SDL_SetRenderDrawColor(game->renderer, 255, 255, 255, 255); 
            SDL_RenderFillRect(game->renderer, &rect);
            SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, 255); 
            SDL_RenderDrawRect(game->renderer, &rect);

            char number_text[10];
            snprintf(number_text, sizeof(number_text), "%d | %d", hand[i].left, hand[i].right);

            TTF_Font* font = TTF_OpenFont("assets/fonts/Arial.ttf", 18);
            if (font) {
                SDL_Color textColor = {0, 0, 0, 255};
                SDL_Surface* surface = TTF_RenderText_Solid(font, number_text, textColor);
                SDL_Texture* texture = SDL_CreateTextureFromSurface(game->renderer, surface);

                SDL_Rect text_rect = {
                    .x = rect.x + 5,
                    .y = rect.y + (rect.h / 2) - (surface->h / 2),
                    .w = surface->w,
                    .h = surface->h
                };

                SDL_RenderCopy(game->renderer, texture, NULL, &text_rect);

                SDL_FreeSurface(surface);
                SDL_DestroyTexture(texture);
                TTF_CloseFont(font);
            }
        }

        
     if (player == 1) { 
    render_player_info(game, player, (game->window_width - (DOMINO_WIDTH + 10) * dominos_state->hand_size) / 2, game->window_height - 100, num_players);
} else if (player == 2) { 
    render_player_info(game, player, (game->window_width - (DOMINO_WIDTH + 10) * dominos_state->hand_size) / 2, 0, num_players);
} else if (player == 3) { 
    render_player_info(game, player, game->window_width - 50, (game->window_height - (DOMINO_WIDTH + 10) * dominos_state->hand_size) / 2, num_players);
} else if (player == 4) { 
    render_player_info(game, player, 50, (game->window_height - (DOMINO_WIDTH + 10) * dominos_state->hand_size) / 2, num_players);
}

    }

    SDL_RenderPresent(game->renderer);
}








