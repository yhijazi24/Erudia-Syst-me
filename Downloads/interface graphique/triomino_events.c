#include <SDL2/SDL.h>
#include "triomino_events.h"
#include "triomino_game.h"
#include "triomino_types.h"

void handle_triomino_events(Game* game, SDL_Event* event) {
    if (event->type == SDL_MOUSEBUTTONDOWN) {
        TriominoGame* triomino_state = get_triomino_game_state();
        int x = event->button.x;
        int y = event->button.y;

        
        for (int player = 0; player < triomino_state->nombreJoueurs; player++) {
            Triomino* hand = triomino_state->player_hands[player];
            while (hand) {
                if (SDL_PointInRect(&(SDL_Point){x, y}, &hand->rect)) {
                    hand->selected = !hand->selected;
                    SDL_Log("Triomino clicked: %d | %d | %d", hand->value1, hand->value2, hand->value3);
                }
                hand = hand->suivant;
            }
        }
    }
}

