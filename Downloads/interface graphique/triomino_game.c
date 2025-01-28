#include "triomino_game.h"
#include "triomino_types.h"

static TriominoGame triomino_state;

TriominoGame* get_triomino_game_state() {
    return &triomino_state;
}
void handle_mouse_click(Game* game, TriominoGame* triomino_state, int x, int y) {
    for (int player = 0; player < triomino_state->nombreJoueurs; player++) {
        Triomino* hand = triomino_state->player_hands[player];
        while (hand) {
            if (x >= hand->rect.x && x <= (hand->rect.x + hand->rect.w) &&
                y >= hand->rect.y && y <= (hand->rect.y + hand->rect.h)) {
                hand->selected = !hand->selected; 
                SDL_Log("Triomino clicked: Player %d, Values: %d | %d | %d", 
                        player + 1, hand->value1, hand->value2, hand->value3);
                return;
            }
            hand = hand->suivant;
        }
    }
}


void triomino_init(Game* game, int num_players) {
    TriominoGame* triomino_state = get_triomino_game_state();
    triomino_state->nombreJoueurs = num_players;
    triomino_state->game_started = true;

    
    triomino_state->playing_area = (SDL_Rect){
        .x = game->window_width / 6,
        .y = game->window_height / 6,
        .w = game->window_width * 2 / 3,
        .h = game->window_height * 2 / 3
    };

    
    for (int i = 0; i < num_players; i++) {
        triomino_state->player_hands[i] = NULL;
        for (int j = 0; j < 6; j++) {  
            Triomino* new_piece = creer_triomino(rand() % 10, rand() % 10, rand() % 10);
            new_piece->suivant = triomino_state->player_hands[i];
            triomino_state->player_hands[i] = new_piece;
        }
    }
}



void triomino_cleanup(Game* game) {
    
    for (int i = 0; i < triomino_state.nombreJoueurs; i++) {
        Triomino* hand = triomino_state.player_hands[i];
        while (hand) {
            Triomino* temp = hand;
            hand = hand->suivant;
            free(temp);
        }
        triomino_state.player_hands[i] = NULL;
    }
}
