//
// Created by kkyse on 1/24/2018.
//

#include "game_server_init.h"

#include <pthread.h>
#include <sys/socket.h>

#include "serialize/buffer.h"
#include "networking.h"
#include "serialize/game_serialization.h"
#include "serialize/player_serialization.h"

static int GameServerOptions_check(const GameServerOptions *options);

static int Game_server_sync_with_client(Game *game, int socket_fd);

static int Game_server_accept(Game *game);

static int Game_server_keep_accepting(Game *game);

static void *Game_server_listen(void *args);

static int GameServerOptions_check(const GameServerOptions *const options) {
    if (!options->port) {
        perror("port not specified");
        return -1;
    }
    
    if (!options->title) {
        perror("title not specified");
        return -1;
    }
    
    if (options->width <= 0) {
        perror("width must be positive");
        return -1;
    }
    
    if (options->height <= 0) {
        perror("height must be positive");
        return -1;
    }
    
    return 0;
}

static int Game_server_sync_with_client(Game *const game, const int socket_fd) {
    const Buffer serialized_game = Game_serialize(game);
    const int send_all_ret_val = send_all(socket_fd, serialized_game);
    free(serialized_game.data);
    if (send_all_ret_val == -1) {
        return -1;
    }
    
    Buffer new_serialized_player = recv_all(socket_fd);
    Player *const new_player = (Player *) malloc(sizeof(Player));
    Player_deserialize(new_player, &new_serialized_player);
    free(new_serialized_player.data);
    
    set_field(new_player->socket_fd, socket_fd);
    if (Game_add_player(game, new_player) == -1) {
        perror("Game_add_player(game, new_player)");
        free(new_player);
        close(socket_fd);
        return -1;
    }
    
    return 0;
}

static int Game_server_accept(Game *const game) {
    const int socket_fd = accept(game->server->listening_socket_fd, NULL, NULL);
    if (socket_fd == -1) {
        perror("accept(game->server->listening_socket_fd, NULL, NULL)");
        return -1;
    }
    
    if (send_acknowledgement(socket_fd) == -1) {
        perror("send_acknowledgement(socket_fd)");
        return -1;
    }
    if (check_acknowledgement(socket_fd) == -1) {
        perror("check_acknowledgement(socket_fd)");
        return -1;
    }
    
    
}

static int Game_server_keep_accepting(Game *const game) {
    for (;;) {
        if (game->quit) {
            return 0;
        }
        
        if (Game_server_accept(game) == -1) {
            return -1;
        }
    }
}

static void *Game_server_listen(void *args) {
    Game *const game = (Game *) args;
    int ret_val = Game_server_keep_accepting(game);
    pthread_exit(&ret_val);
}

static GameServer GameServer_create(const Game *const game, const char *const port) {
    const GameServer invalid_server = {
            .listening_socket_fd = -1
    };
    
    const int socket_fd = listen_to_socket(port);
    if (socket_fd == -1) {
        perror("listen_to_socket(port)");
        return invalid_server;
    }
    
    pthread_t thid;
    if (pthread_create(&thid, NULL, Game_server_listen, (void *) game) != 0) {
        perror("GameServer Listening Thread Creation Failed:"
                       "pthread_create(&thid, NULL, Game_server_listen, (void *) game)");
        close(socket_fd);
        return invalid_server;
    }
    
    const GameServer server = {
            .listening_socket_fd = socket_fd,
            .listening_thread = thid,
    };
    return server;
}

int Game_server_init(Game *const game, const GameServerOptions *const options) {
    const Game local;
    if (game < &local) {
        /*
         * checks if game is on stack, b/c last local variable is highest on the stack
         * if declared on heap, for example, it will after the entire stack
         *
         * Since another thread will be accessing this game to accept new client connections,
         * the game must not be allocated on the stack.
         * It could be a global variable, but it's easier to just require it to be on the heap.
         */
        perror("game must be allocated on heap, shared, or mmap'ed memory, not on stack");
        return -1;
    }
    
    if (GameServerOptions_check(options) == -1) {
        perror("Invalid GameServerOptions: GameServerOptions_check(options)");
        return -1;
    }
    
    const String title = {
            .chars = (char *) options->title,
            .length = strlen(options->title)
    };
    
    GameServer *const heap_server = (GameServer *) realloc(game->server, sizeof(GameServer));
    if (!heap_server) {
        perror("Malloc Failed: realloc(game->server, sizeof(GameServer))");
        return -1;
    }
    
    const GameServer server = GameServer_create(game, options->port);
    if (server.listening_socket_fd == -1) {
        perror("GameServer_create(game, options->port)");
        free(heap_server);
        return -1;
    }
    memcpy(heap_server, &server, sizeof(server));
    
    set_field(game->render, false);
    set_field(game->is_client, false);
    set_field(game->client, NULL);
    set_field(game->server, heap_server);
    set_field(game->title, title);
    
    game->prev_time = 0;
    game->interrupt = NULL;
    game->is_running = true;
    game->quit = false;
    
    game->fps = options->fps;
    game->tick = 0;
    
    GameState *const state = &game->state;
    set_field(state->_x, 0);
    set_field(state->_y, 0);
    set_field(state->width, options->width);
    set_field(state->height, options->height);
    
    const Vector size = Vector_new(state->width, state->height);
    set_field(state->size, size);
    
    set_field(state->own_player, NULL);
    
    const Players players = {
            .max_num_players = options->max_num_players,
            .num_players = 0,
            .players = NULL,
    };
    set_field(state->players, players);
    
    return 0;
}

int Game_server_main(const int argc, const char *const *const argv) {
    // TODO
    return 0;
}