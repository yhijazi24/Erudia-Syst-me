#ifndef PLAYER_SETUP_H
#define PLAYER_SETUP_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>  
#include "menu.h"

typedef struct {
    int nombreJoueurs;
    char nomsJoueurs[4][20]; 
    int joueurIA;            
} ConfigurationJoueurs;

void afficher_configuration_joueurs(SDL_Renderer *renderer, ConfigurationJoueurs *config);
void gerer_clic_configuration(SDL_Event *event, ConfigurationJoueurs *config, GameState *current_state, GameMode *game_mode);


void render_button(SDL_Renderer *renderer, TTF_Font *font, const char *text, SDL_Rect rect, SDL_Color text_color, SDL_Color border_color);

#endif
