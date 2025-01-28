#include "triomino_types.h"
#include <stdlib.h>

Triomino* creer_triomino(int v1, int v2, int v3) {
    Triomino* new_triomino = (Triomino*)malloc(sizeof(Triomino));
    if (!new_triomino) {
        return NULL; 
    }

    new_triomino->value1 = v1;
    new_triomino->value2 = v2;
    new_triomino->value3 = v3;
    new_triomino->selected = false;
    new_triomino->visible = true;
    new_triomino->suivant = NULL; 
    new_triomino->rect = (SDL_Rect){0, 0, 0, 0}; 
    return new_triomino;
}
