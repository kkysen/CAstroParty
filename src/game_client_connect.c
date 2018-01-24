//
// Created by kkyse on 1/24/2018.
//

#include "game_client_connect.h"

#include "util/utils.h"
#include "serialize/game_serialization.h"
#include "serialize/player_serialization.h"
#include "util/sdl_utils.h"

static int Game_client_fork_server(Game *game, IpPort ip_port);

static int GameClient_init_window(GameClient *client, const Game *game);

static int GameClient_init_renderer(GameClient *client);

static int GameClient_init(GameClient *client, const Game *game);

static void GameClient_destroy(GameClient *client);

static int GameState_load_sprites(GameState *state, SDL_Renderer *renderer);

static int Game_client_init_graphics(Game *game);

static int Game_client_add_own_player_with_name_and_texture(
        Game *game, int socket_fd, String name, GameTexture texture);

static int Game_client_add_own_player(Game *game, int socket_fd);

static int Game_client_fork_server(Game *const game, const IpPort ip_port) {
    const int socket_fd = connect_to_socket(ip_port);
    Buffer server_game = recv_all(socket_fd);
    if (server_game.length == -1) {
        perror("recv_all(socket_fd)");
        check_perror(close(socket_fd));
        return -1;
    }
    
    // deserialization sets default values for a client
    Game_deserialize(game, &server_game);
    return socket_fd;
}

static int GameClient_init_window(GameClient *const client, const Game *const game) {
    const SDL_Window *const window = SDL_CreateWindow(
            game->title.chars,
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            game->state.width,
            game->state.height,
            SDL_WINDOW_OPENGL
    );
    sdl_check_null_perror_msg(
            window,
            "SDL_CreateWindow(game->title.chars, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, game->state.width, game->state.height, SDL_WINDOW_OPENGL)");
    set_field(client->window, window);
    return 0;
}

static int GameClient_init_renderer(GameClient *const client) {
    const SDL_Renderer *const renderer =
            SDL_CreateRenderer(client->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    sdl_check_null_perror_msg(
            renderer, "SDL_CreateRenderer(client->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC)");
    set_field(client->renderer, renderer);
    return 0;
}

static int GameClient_init(GameClient *const client, const Game *const game) {
    sdl_check_perror(GameClient_init_window(client, game));
    if (GameClient_init_renderer(client) == -1) {
        GameClient_destroy(client);
        sdl_perror("GameClient_init_renderer(game)");
        return -1;
    }
    return 0;
}

static void GameClient_destroy(GameClient *const client) {
    if (client->window) {
        SDL_DestroyWindow(client->window);
        set_field(client->window, NULL);
    }
    
    if (client->renderer) {
        SDL_DestroyRenderer(client->renderer);
        set_field(client->renderer, NULL);
    }
}

static int GameState_load_sprites(GameState *const state, SDL_Renderer *const renderer) {
    if (Players_load_sprites(state->players, renderer) == -1) {
        perror("Players_load_sprites(state->players, renderer)");
        // sprites are cleaned up atexit()
        return -1;
    }
    
    return 0;
}

static int Game_client_init_graphics(Game *const game) {
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        sdl_perror("SDL_Init(SDL_INIT_EVERYTHING)");
        if (str_contains(SDL_GetError(), "No available video device")) {
            sdl_log_func(stderr, SDL_GetNumVideoDrivers(), 1);
            sdl_log_func(stderr, SDL_GetNumVideoDisplays(), 1);
        }
        return -1;
    }
    
    if (GameClient_init(game->client, game) == -1) {
        sdl_perror("GameClient_init(game->client, game)");
        goto error;
    }
    
    if (GameState_load_sprites(&game->state, game->client->renderer) == -1) {
        perror("GameState_load_sprites(&game->state)");
        goto error;
    }
    return 0;
    
    error:
    GameClient_destroy(game->client);
    SDL_Quit();
    return -1;
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
    
    memcpy((int *) &own_player->socket_fd, &socket_fd, sizeof(int));
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
    
    if (Game_client_init_graphics(game) == -1) {
        perror("Game_client_init_graphics(game)");
        goto error;
    }
    
    if (Game_client_add_own_player(game, socket_fd) == -1) {
        perror("Game_client_add_own_player(game, socket_fd)");
        goto error;
    }
    
    return 0;
    
    error:
    Game_destroy(game); // FIXME Game_destroy() must be updated
    close(socket_fd);
    return -1;
}