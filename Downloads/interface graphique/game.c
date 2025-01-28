#include "game.h"
#include "menu.h"
#include <string.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include "dominos_game.h"
#include "triomino_game.h"
#include "player_setup.h"
#include "dominos_renderer.h"
#include "triomino_renderer.h"
#include "triomino_events.h"

static ConfigurationJoueurs config;

bool game_init(Game* game) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_Log("SDL initialization failed: %s", SDL_GetError());
        return false;
    }

    if (TTF_Init() < 0) {
        SDL_Log("SDL_ttf initialization failed: %s", TTF_GetError());
        return false;
    }

    game->window_width = 1200;
    game->window_height = 800;
    game->window = SDL_CreateWindow(
        "Dominos & Triominos",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        game->window_width,
        game->window_height,
        SDL_WINDOW_SHOWN
    );

    if (!game->window) {
        SDL_Log("Window creation failed: %s", SDL_GetError());
        return false;
    }

    game->renderer = SDL_CreateRenderer(
        game->window,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );

    if (!game->renderer) {
        SDL_Log("Renderer creation failed: %s", SDL_GetError());
        return false;
    }

    if (!menu_init(game)) {
        SDL_Log("Menu initialization failed");
        return false;
    }

    game->state = GAME_STATE_MENU;
    game->game_mode = GAME_MODE_NONE; 
    game->running = true;

    
    config.nombreJoueurs = 2;
    strcpy(config.nomsJoueurs[0], "");
    strcpy(config.nomsJoueurs[1], "");
    config.joueurIA = 0;

    return true;
}

void game_run(Game* game) {
    SDL_Event event;
    Uint32 frame_start, frame_time;
    bool needs_update = true; 

    while (game->running) {
        frame_start = SDL_GetTicks(); 

        
        while (SDL_PollEvent(&event)) {
            
            if (event.type == SDL_QUIT) {
                game->running = false; 
                break;
            }
            switch (game->state) {
                case GAME_STATE_MENU:
                    handle_menu_events(game, &event);
                    needs_update = true;
                    break;

                case CONFIG_JOUEURS:
                    gerer_clic_configuration(&event, &config, &game->state, &game->game_mode);
                    needs_update = true;
                    break;

                case GAME_STATE_DOMINOS:
                    DominosGame* dominos_state = get_dominos_game_state();
                    if (!dominos_state->game_started) {
                        dominos_init(game, config.nombreJoueurs); 
                        dominos_state->game_started = true;
                        needs_update = true;
                    }
                    handle_dominos_events(game, &event);
                    needs_update = true;
                    break;

                case GAME_STATE_TRIOMINOS:
                    TriominoGame* triomino_state = get_triomino_game_state();
                    if (!triomino_state->game_started) {
                        triomino_init(game, config.nombreJoueurs); 
                        triomino_state->game_started = true;
                        needs_update = true;
                    }
                    handle_triomino_events(game, &event);
                    needs_update = true;
                    break;

                case GAME_STATE_QUIT:
                    game->running = false;
                    break;

                default:
                    break;
            }
        }

        
        if (needs_update) {
            SDL_SetRenderDrawColor(game->renderer, 40, 44, 52, 255); 
            SDL_RenderClear(game->renderer);

            switch (game->state) {
                case GAME_STATE_MENU:
                    render_menu(game);
                    break;

                case CONFIG_JOUEURS:
                    afficher_configuration_joueurs(game->renderer, &config);
                    needs_update = false;
                    break;

                case GAME_STATE_DOMINOS:
                    render_dominos(game, config.nombreJoueurs); 
                    break;

                case GAME_STATE_TRIOMINOS:
                    render_triominos(game);
                    break;

                case GAME_STATE_QUIT:
                    break;

                default:
                    break;
            }

            SDL_RenderPresent(game->renderer); 
            needs_update = false; 
        }

        
        frame_time = SDL_GetTicks() - frame_start;
        if (frame_time < 16) { 
            SDL_Delay(16 - frame_time);
        }
    }
}

void game_cleanup(Game* game) {
    dominos_cleanup(game);
    triomino_cleanup(game);
    menu_cleanup(game);
    SDL_DestroyRenderer(game->renderer);
    SDL_DestroyWindow(game->window);
    TTF_Quit();
    SDL_Quit();
}
