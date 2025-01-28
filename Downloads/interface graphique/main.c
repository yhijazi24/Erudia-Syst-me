#define SDL_MAIN_HANDLED
#include <stdio.h>
#include "game.h"

int main(int argc, char* argv[]) {
    (void)argc;
    (void)argv;
    
    Game game = {0};
    
    if (!game_init(&game)) {
        fprintf(stderr, "Failed to initialize game\n");
        return 1;
    }
    
    game_run(&game);
    game_cleanup(&game);
    
    return 0;
}