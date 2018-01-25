//
// Created by kkyse on 1/24/2018.
//

#ifndef CASTROPARTY_GAME_SERVER_INIT_H
#define CASTROPARTY_GAME_SERVER_INIT_H

#include "game2.h"

typedef struct {
    const char *const port;
    const char *const title;
    const uint8_t fps;
    const int width;
    const int height;
    const uint8_t max_num_players;
} GameServerOptions;

/**
 * Initialize a server \param game
 * with options specified in \param options.
 * This includes a port to listen on for client connections.
 *
 * Note: \param game must not be declared on the stack,
 * because multiple threads use it.
 *
 * @param game the uninitialized server game
 * @param options the options for setting up the game server
 * @return -1 if error
 */
int Game_server_init(Game *game, const GameServerOptions *options);

/**
 * Start a game server according to command line args, like in main().
 *
 * TODO explain CLI
 *
 * @param argc number of arguments (as in main())
 * @param argv argument vector (as in main())
 * @return -1 if error
 */
int Game_server_main(int argc, const char *const *argv);

#endif // CASTROPARTY_GAME_SERVER_INIT_H
