#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "triomino_renderer.h"
#include "triomino_game.h"
#include "triomino_types.h"
#include <stdio.h>

#define TRIOMINO_SIZE 90
#define SPACING 20


void draw_filled_triangle(SDL_Renderer* renderer, int x, int y, int size, bool rotate) {
    int base_offset = size / 8;

    SDL_Vertex vertices[3] = {
        {{x, y}, {255, 255, 255, 255}, {0, 0}},  
        {{x - (size / 2 + base_offset), y + size}, {255, 255, 255, 255}, {0, 0}}, 
        {{x + (size / 2 + base_offset), y + size}, {255, 255, 255, 255}, {0, 0}}  
    };

    SDL_RenderGeometry(renderer, NULL, vertices, 3, NULL, 0);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderDrawLine(renderer, vertices[0].position.x, vertices[0].position.y,
                       vertices[1].position.x, vertices[1].position.y);
    SDL_RenderDrawLine(renderer, vertices[1].position.x, vertices[1].position.y,
                       vertices[2].position.x, vertices[2].position.y);
    SDL_RenderDrawLine(renderer, vertices[2].position.x, vertices[2].position.y,
                       vertices[0].position.x, vertices[0].position.y);
}


void dessiner_triomino(SDL_Renderer* renderer, TTF_Font* font, Triomino* triomino, int x, int y, int size, bool rotate) {
    if (!triomino) return;

    draw_filled_triangle(renderer, x, y, size, false); 

    SDL_Color textColor = {0, 0, 0, 255};
    char values[3][5];

    snprintf(values[0], sizeof(values[0]), "%d", triomino->value1);
    snprintf(values[1], sizeof(values[1]), "%d", triomino->value2);
    snprintf(values[2], sizeof(values[2]), "%d", triomino->value3);

    SDL_Surface* surface;
    SDL_Texture* texture;
    SDL_Rect text_rect;

    int text_positions[3][2] = {
        {x - size / 3, y + (size * 3) / 4}, 
        {x + size / 3, y + (size * 3) / 4}, 
        {x, y + size / 4}                   
    };

    for (int i = 0; i < 3; i++) {
        surface = TTF_RenderText_Blended(font, values[i], textColor);
        texture = SDL_CreateTextureFromSurface(renderer, surface);

        text_rect = (SDL_Rect){
            .x = text_positions[i][0] - surface->w / 2,
            .y = text_positions[i][1] - surface->h / 2,
            .w = surface->w,
            .h = surface->h
        };

        SDL_RenderCopy(renderer, texture, NULL, &text_rect);
        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);
    }
}


void render_triomino_plateau(Game* game, TriominoGame* triomino_state) {
    SDL_SetRenderDrawColor(game->renderer, 34, 139, 34, 255); 
    SDL_RenderClear(game->renderer);

    SDL_SetRenderDrawColor(game->renderer, 169, 169, 169, 255); 
    SDL_RenderFillRect(game->renderer, &triomino_state->playing_area);

    TTF_Font* font = TTF_OpenFont("assets/fonts/Arial.ttf", 26);
    if (!font) {
        SDL_Log("Failed to load font for rendering: %s", TTF_GetError());
        return;
    }

    int spacing = TRIOMINO_SIZE + SPACING;
    int triominoes_per_player = (triomino_state->nombreJoueurs >= 3) ? 6 : 7;

    for (int player = 1; player <= triomino_state->nombreJoueurs; player++) {
        Triomino* hand = triomino_state->player_hands[player - 1];

        int x, y;
        bool rotate_text = false;
        if (player == 1) {
            x = (game->window_width - spacing * triominoes_per_player) / 2 + 115;
            y = game->window_height - TRIOMINO_SIZE - 10;
        } else if (player == 2) {
            x = (game->window_width - spacing * triominoes_per_player) / 2 + 115;
            y = 10;
        } else if (player == 3) {
            x = game->window_width - TRIOMINO_SIZE - 15;
            y = (game->window_height - spacing * triominoes_per_player) / 2;
            rotate_text = true; 
        } else if (player == 4) {
            x = 110;
            y = (game->window_height - spacing * triominoes_per_player) / 2;
            rotate_text = true; 
        }

        int i = 0;
        while (hand) {
            dessiner_triomino(game->renderer, font, hand, x + (player <= 2 ? i * spacing : 0), y + (player > 2 ? i * spacing : 0), TRIOMINO_SIZE, false);
            hand = hand->suivant;
            i++;
        }

        
        SDL_Color white = {255, 255, 255, 255};
        char player_info[50];
        snprintf(player_info, sizeof(player_info), "Player %d: %d", player, 0); 

        SDL_Surface* text_surface = TTF_RenderText_Blended(font, player_info, white);
        SDL_Texture* text_texture = SDL_CreateTextureFromSurface(game->renderer, text_surface);

        SDL_Rect text_rect;
        if (player == 1) {
            text_rect = (SDL_Rect){x - 200, y + 20, text_surface->w, text_surface->h};  
        } else if (player == 2) {
            text_rect = (SDL_Rect){x - 200, y + 20, text_surface->w, text_surface->h};  
        } else if (player == 3) {
            text_rect = (SDL_Rect){x + 20, y + (spacing * triominoes_per_player) / 2, text_surface->w, text_surface->h}; 
        } else if (player == 4) {
            text_rect = (SDL_Rect){x - 150, y + (spacing * triominoes_per_player) / 2, text_surface->w, text_surface->h}; 
        }

        SDL_RenderCopyEx(game->renderer, text_texture, NULL, &text_rect, (rotate_text ? (player == 3 ? -90 : 90) : 0), NULL, SDL_FLIP_NONE);

        SDL_FreeSurface(text_surface);
        SDL_DestroyTexture(text_texture);
    }

    SDL_RenderPresent(game->renderer);
    TTF_CloseFont(font);
}

void render_triominos(Game* game) {
    TriominoGame* triomino_state = get_triomino_game_state();
    render_triomino_plateau(game, triomino_state);
}
