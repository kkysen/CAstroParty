//
// Created by kkyse on 12/27/2017.
//

#include "client.h"

#include <stdbool.h>

#include "game.h"
#include "util/utils.h"
#include "util/sdl_utils.h"
#include "util/stacktrace.h"
#include "player.h"

int run(const float orientation) {
    //    set_stack_trace_signal_handler();
    Game *const game = _Game_new(true);
    sdl_check_null_perror_msg(game, "Game_new(true)");
    p("Created game");
    Player *const player = Player_new("Khyber", BLUE_PLAYER);
    Vector_i_mul_add(player->position, game->state.size, 0.25f);
    player->orientation = orientation;
    pp(player->sprite.texture);
    check_null_msg(player, "Player_new(\"Khyber\", BLUE_PLAYER)");
    p("Created player");
    check_perror(Game_add_player(game, player));
    p("Running");
    Game_quit_later(game, 10);
    Game_run(game);
    p("Finished");
    free(player);
    free(game);
    return 0;
}

int main() {
    run(45);
}