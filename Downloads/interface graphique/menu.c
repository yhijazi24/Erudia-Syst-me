#include <SDL2/SDL_ttf.h>
#include "game.h"
#include "menu.h"
#include "dominos_game.h"

#define BUTTON_WIDTH 200
#define BUTTON_HEIGHT 50
#define BUTTON_PADDING 20

typedef struct {
    SDL_Rect rect;
    const char* text;
    GameState target_state;
} Button;

static Button buttons[2];
static TTF_Font* font = NULL;

bool menu_init(Game* game) {
    
    font = TTF_OpenFont("assets/fonts/Arial.ttf", 24);
    if (!font) {
        SDL_Log("Failed to load font: %s", TTF_GetError());
        return false;
    }

    
    int total_height = 2 * BUTTON_HEIGHT + BUTTON_PADDING;
    int start_y = (game->window_height - total_height) / 2;

    
    buttons[0].rect.w = BUTTON_WIDTH;
    buttons[0].rect.h = BUTTON_HEIGHT;
    buttons[0].rect.x = (game->window_width - BUTTON_WIDTH) / 2;
    buttons[0].rect.y = start_y;
    buttons[0].text = "Dominos";
    buttons[0].target_state = CONFIG_JOUEURS;

    
    buttons[1].rect.w = BUTTON_WIDTH;
    buttons[1].rect.h = BUTTON_HEIGHT;
    buttons[1].rect.x = (game->window_width - BUTTON_WIDTH) / 2;
    buttons[1].rect.y = start_y + BUTTON_HEIGHT + BUTTON_PADDING;
    buttons[1].text = "Triominos";
    buttons[1].target_state = CONFIG_JOUEURS;

    return true;
}

void menu_cleanup(Game* game) {
    (void)game;
    if (font) {
        TTF_CloseFont(font);
        font = NULL;
    }
}

void handle_menu_events(Game* game, SDL_Event* event) {
    if (event->type == SDL_MOUSEBUTTONDOWN) {
        int mouseX = event->button.x;
        int mouseY = event->button.y;

        for (int i = 0; i < 2; i++) {
            if (mouseX >= buttons[i].rect.x &&
                mouseX <= buttons[i].rect.x + buttons[i].rect.w &&
                mouseY >= buttons[i].rect.y &&
                mouseY <= buttons[i].rect.y + buttons[i].rect.h) {
                
                
                game->game_mode = (i == 0) ? GAME_MODE_DOMINOS : GAME_MODE_TRIOMINOS;

                
                game->state = CONFIG_JOUEURS;
                break;
            }
        }
    }
}


void render_menu(Game* game) {
    SDL_Color white = {255, 255, 255, 255};

    
    SDL_Surface* title_surface = TTF_RenderText_Blended(font, "Choose Your Game", white);
    SDL_Texture* title_texture = SDL_CreateTextureFromSurface(game->renderer, title_surface);

    SDL_Rect title_rect = {
        .x = (game->window_width - title_surface->w) / 2,
        .y = 50,
        .w = title_surface->w,
        .h = title_surface->h
    };

    SDL_RenderCopy(game->renderer, title_texture, NULL, &title_rect);
    SDL_FreeSurface(title_surface);
    SDL_DestroyTexture(title_texture);

    
    for (int i = 0; i < 2; i++) {
        SDL_SetRenderDrawColor(game->renderer, 70, 70, 70, 255);
        SDL_RenderFillRect(game->renderer, &buttons[i].rect);

        SDL_SetRenderDrawColor(game->renderer, 200, 200, 200, 255);
        SDL_RenderDrawRect(game->renderer, &buttons[i].rect);

        SDL_Surface* text_surface = TTF_RenderText_Blended(font, buttons[i].text, white);
        SDL_Texture* text_texture = SDL_CreateTextureFromSurface(game->renderer, text_surface);

        SDL_Rect text_rect = {
            .x = buttons[i].rect.x + (BUTTON_WIDTH - text_surface->w) / 2,
            .y = buttons[i].rect.y + (BUTTON_HEIGHT - text_surface->h) / 2,
            .w = text_surface->w,
            .h = text_surface->h
        };

        SDL_RenderCopy(game->renderer, text_texture, NULL, &text_rect);
        SDL_FreeSurface(text_surface);
        SDL_DestroyTexture(text_texture);
    }
}
