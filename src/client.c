//
// Created by kkyse on 12/27/2017.
//

#include "client.h"

#include <stdbool.h>

#include "game.h"
#include "util/utils.h"
#include "util/sdl_utils.h"

int main() {
    Game *const game = Game_new(true);
    sdl_check_null_perror_msg(game, "Game_new(true)");
    p("Created game");
    Player *const player = Player_new("Khyber", BLUE_PLAYER);
    check_null_msg(player, "Player_new(\"Khyber\", BLUE_PLAYER)");
    p("Created player");
    check_perror(Game_add_player(game, player));
    p("Running");
    Game_run(game);
    p("Finished");
    free(player);
    free(game);
    return 0;
}