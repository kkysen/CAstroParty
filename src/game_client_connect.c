//
// Created by kkyse on 1/24/2018.
//

#include "game_client_connect.h"

#include "util/utils.h"

static int Game_client_fork_server(Game *game, IpPort ip_port);

static int Game_client_init_fields(Game *game);

static int Game_client_add_own_player_with_name_and_texture(
        Game *game, int socket_fd, String name, GameTexture texture);

static int Game_client_add_own_player(Game *game, int socket_fd);

static int Game_client_fork_server(Game *const game, const IpPort ip_port) {
    const int socket_fd = connect_to_socket(ip_port);
    const Buffer server_game = recv_all(socket_fd);
    if (server_game.length == -1) {
        perror("recv_all(socket_fd)");
        goto error;
    }
    if (Game_deserialize(game, server_game) == -1) {
        perror("Game_deserialize(game, server_game)");
        free(server_game.data);
        goto error;
    }
    return socket_fd;
    
    error:
    check_perror(close(socket_fd));
    return -1;
}

static int Game_client_init_fields(Game *const game) {
    const bool is_client = true;
    memcpy((bool *) &game->is_client, &is_client, sizeof(bool));
    game->interrupt = NULL;
    game->prev_time = 0;
    
    Players_invalidate_sockets(game->state.players);
    GameState_reload_sprites(game->state);
    Game_init_graphics(game);
    
    return 0;
}

static int Game_client_add_own_player_with_name_and_texture(
        Game *const game, const int socket_fd, const String name, const GameTexture texture) {
    Player *const own_player = Player_new(name, texture);
    
    const Buffer serialized_player = Player_serialize(own_player);
    
    if (send_all(socket_fd, serialized_player) == -1) {
        perror("send_all(socket_fd, serialized_player)");
        goto error;
    }
    
    if (check_acknowledgement(socket_fd) == -1) {
        perror("check_acknowledgement(socket_fd)");
        goto error;
    }
    
    if (Game_add_player(game, own_player) == -1) {
        perror("Game_add_player(game, own_player)");
        goto error;
    }
    
    memcpy((Player *) &game->state.own_player, own_player, sizeof(Player));
    return 0;
    
    error:
    free(own_player);
    free(serialized_player.data);
    return -1;
}

static int Game_client_add_own_player(Game *const game, const int socket_fd) {
    const String name = {0}; // FIXME should be read from user input
    const GameTexture texture = BLUE_PLAYER; // FIXME should be selected by user
    
    if (Game_client_add_own_player_with_name_and_texture(game, socket_fd, name, texture) == -1) {
        perror("Game_client_add_own_player_with_name_and_texture(game, socket_fd, name, texture)");
        // free name
        return -1;
    }
    return 0;
}

int Game_client_connect(Game *game, IpPort ip_port) {
    const int socket_fd = Game_client_fork_server(game, ip_port);
    if (socket_fd == -1) {
        perror("Game_client_fork_server(game, ip_port)");
        return -1;
    }
    
    if (Game_client_init_fields(game) == -1) {
        perror("Game_client_init_fields(game)");
        goto error;
    }
    
    if (Game_client_add_own_player(game, socket_fd) == -1) {
        perror("Game_client_add_own_player(game, socket_fd)");
        goto error;
    }
    
    return 0;
    
    error:
    close(socket_fd);
    return -1;
}