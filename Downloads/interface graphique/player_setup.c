#include "player_setup.h"
#include <SDL2/SDL_ttf.h>
#include <string.h>
#include <stdio.h>


SDL_Color white = {255, 255, 255, 255};
SDL_Color gray = {70, 70, 70, 255};
SDL_Color lightGray = {200, 200, 200, 255};


void render_text(SDL_Renderer *renderer, TTF_Font *font, const char *text, int x, int y, SDL_Color color) {
    SDL_Surface *surface = TTF_RenderText_Blended(font, text, color);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect rect = {x, y, surface->w, surface->h};
    SDL_RenderCopy(renderer, texture, NULL, &rect);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}


void render_button(SDL_Renderer *renderer, TTF_Font *font, const char *text, SDL_Rect rect, SDL_Color text_color, SDL_Color border_color) {
    SDL_SetRenderDrawColor(renderer, gray.r, gray.g, gray.b, 255);
    SDL_RenderFillRect(renderer, &rect);
    SDL_SetRenderDrawColor(renderer, border_color.r, border_color.g, border_color.b, 255);
    SDL_RenderDrawRect(renderer, &rect);
    
    SDL_Surface *surface = TTF_RenderText_Blended(font, text, text_color);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect text_rect = {
        .x = rect.x + (rect.w - surface->w) / 2,
        .y = rect.y + (rect.h - surface->h) / 2,
        .w = surface->w,
        .h = surface->h
    };
    SDL_RenderCopy(renderer, texture, NULL, &text_rect);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}


void afficher_configuration_joueurs(SDL_Renderer *renderer, ConfigurationJoueurs *config) {
    SDL_SetRenderDrawColor(renderer, 34, 39, 46, 255); 
    SDL_RenderClear(renderer);

    TTF_Font *font = TTF_OpenFont("assets/fonts/arial.ttf", 24);
    if (!font) {
        SDL_Log("Failed to load font: %s", TTF_GetError());
        return;
    }

    int window_width = 1200;

    
    render_text(renderer, font, "Player Setup", window_width / 2 - 100, 100, white);

    
    char text[50];
    snprintf(text, sizeof(text), "Number of Players: %d", config->nombreJoueurs);
    render_text(renderer, font, text, window_width / 2 - 150, 150, white);

    
    int input_x = window_width / 2 - 150;
    int input_start_y = 200;
    int input_width = 300;
    int input_height = 50;

    for (int i = 0; i < config->nombreJoueurs; i++) {
        SDL_Rect input_box = {input_x, input_start_y + i * 70, input_width, input_height};

        
        SDL_SetRenderDrawColor(renderer, gray.r, gray.g, gray.b, 255);
        SDL_RenderFillRect(renderer, &input_box);
        SDL_SetRenderDrawColor(renderer, white.r, white.g, white.b, 255);
        SDL_RenderDrawRect(renderer, &input_box);

        
        snprintf(text, sizeof(text), "Player %d: %s", i + 1, 
                 config->nomsJoueurs[i][0] ? config->nomsJoueurs[i] : "Enter Username");
        render_text(renderer, font, text, input_x + 10, input_box.y + 15, white);
    }

    
    int button_x = window_width / 2 - 125;
    int button_start_y = input_start_y + config->nombreJoueurs * 70 + 30;
    SDL_Rect buttons[5] = {
        {button_x, button_start_y, 250, 50},
        {button_x, button_start_y + 60, 250, 50},
        {button_x, button_start_y + 120, 250, 50},
        {button_x, button_start_y + 180, 250, 50},
        {button_x, button_start_y + 240, 250, 50}
    };

    
    render_button(renderer, font, "Increase [+]", buttons[0], white, lightGray);
    render_button(renderer, font, "Decrease [-]", buttons[1], white, lightGray);
    render_button(renderer, font, config->joueurIA ? "Disable AI" : "Enable AI", buttons[2], white, lightGray);
    render_button(renderer, font, "Start Game", buttons[3], white, lightGray);
    render_button(renderer, font, "Back to Menu", buttons[4], white, lightGray);

    TTF_CloseFont(font);
    SDL_RenderPresent(renderer);
}



void gerer_clic_configuration(SDL_Event *event, ConfigurationJoueurs *config, GameState *current_state, GameMode *game_mode) {
    static int current_player = -1;  

    int window_width = 1200;
    int button_x = window_width / 2 - 125;
    int button_start_y = 200 + config->nombreJoueurs * 70 + 30;

    SDL_Rect buttons[5] = {
        {button_x, button_start_y, 250, 50},
        {button_x, button_start_y + 60, 250, 50},
        {button_x, button_start_y + 120, 250, 50},
        {button_x, button_start_y + 180, 250, 50},
        {button_x, button_start_y + 240, 250, 50}
    };

    bool state_changed = false;

    
    if (event->type == SDL_MOUSEBUTTONDOWN) {
        int x = event->button.x;
        int y = event->button.y;

        
        for (int i = 0; i < config->nombreJoueurs; i++) {
            SDL_Rect input_box = {button_x, 200 + i * 70, 300, 50};  
            if (x >= input_box.x && x <= input_box.x + input_box.w &&
                y >= input_box.y && y <= input_box.y + input_box.h) {
                current_player = i;  
                SDL_StartTextInput();  
                return;
            }
        }

        
        if (x >= buttons[0].x && x <= buttons[0].x + buttons[0].w && y >= buttons[0].y && y <= buttons[0].y + buttons[0].h) {
            if (config->nombreJoueurs < 4) config->nombreJoueurs++;
            state_changed = true;
        } else if (x >= buttons[1].x && x <= buttons[1].x + buttons[1].w && y >= buttons[1].y && y <= buttons[1].y + buttons[1].h) {
            if (config->nombreJoueurs > 2) config->nombreJoueurs--;
            state_changed = true;
        } else if (x >= buttons[2].x && x <= buttons[2].x + buttons[2].w && y >= buttons[2].y && y <= buttons[2].y + buttons[2].h) {
            config->joueurIA = !config->joueurIA;
            if (config->joueurIA && config->nombreJoueurs < 4) {
                strcpy(config->nomsJoueurs[config->nombreJoueurs++], "AI");
            }
            state_changed = true;
        } else if (x >= buttons[3].x && x <= buttons[3].x + buttons[3].w && y >= buttons[3].y && y <= buttons[3].y + buttons[3].h) {
            *current_state = (*game_mode == GAME_MODE_DOMINOS) ? GAME_STATE_DOMINOS : GAME_STATE_TRIOMINOS;
        } else if (x >= buttons[4].x && x <= buttons[4].x + buttons[4].w && y >= buttons[4].y && y <= buttons[4].y + buttons[4].h) {
            *current_state = GAME_STATE_MENU;
        }
    }

    
    if (event->type == SDL_TEXTINPUT && current_player >= 0) {
        
        strncat(config->nomsJoueurs[current_player], event->text.text,
                sizeof(config->nomsJoueurs[current_player]) - strlen(config->nomsJoueurs[current_player]) - 1);
    }

    
    if (event->type == SDL_KEYDOWN && current_player >= 0) {
        if (event->key.keysym.sym == SDLK_BACKSPACE) {
            size_t len = strlen(config->nomsJoueurs[current_player]);
            if (len > 0) config->nomsJoueurs[current_player][len - 1] = '\0';
        }
    }

    
    if (state_changed) {
        afficher_configuration_joueurs(SDL_GetRenderer(SDL_GetWindowFromID(1)), config);
    }
}
