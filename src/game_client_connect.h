//
// Created by kkyse on 1/24/2018.
//

#ifndef CASTROPARTY_GAME_CLIENT_CONNECT_H
#define CASTROPARTY_GAME_CLIENT_CONNECT_H

#include "networking.h"
#include "game2.h"

/**
 * Connects a client \param game to a server at the IP address and port \param ip_port.
 * It forks the server's current state,
 * updates it to conform to a client,
 * adds its own player,
 * and sends the new player to the server.
 *
 * @param game
 * @param ip_port the IP address and port of the server to connect to to
 * @return -1 if error
 */
int Game_client_connect(Game *game, IpPort ip_port);

#endif // CASTROPARTY_GAME_CLIENT_CONNECT_H
