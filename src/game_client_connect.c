//
// Created by kkyse on 1/24/2018.
//

#include "game_client_connect.h"

#include "util/utils.h"
#include "serialize/game_serialization.h"
#include "serialize/player_serialization.h"
#include "util/sdl_utils.h"
#include "client_init_input.h"
#include "serialize/buffer.h"
#include "player.h"

static uint8_t MAX_PLAYER_CHOOSES = 100;

static int Game_client_fork_server(Game *game, IpPort ip_port);

static int GameClient_init_window(GameClient *client, const Game *game);

static int GameClient_init_renderer(GameClient *client);

static int GameClient_init(GameClient *client, const Game *game);

static void GameClient_destroy(GameClient *client);

static int GameState_load_sprites(GameState *state, SDL_Renderer *renderer);

static int Game_client_init_graphics(Game *game);

static String get_player_name(const Players *players);

static GameTexture get_player_texture(const Players *players);

static int Game_client_add_own_player_with_name_and_texture(
        Game *game, int socket_fd, String name, GameTexture texture);

static int Game_client_add_own_player(Game *game, int socket_fd);

static int Game_client_fork_server(Game *const game, const IpPort ip_port) {
    const int socket_fd = connect_to_socket(ip_port);
    
    if (send_acknowledgement(socket_fd) == -1) {
        perror("send_acknowledgement(socket_fd)");
        return -1;
    }
    if (check_acknowledgement(socket_fd) == -1) {
        perror("check_acknowledgement(socket_fd)");
        return -1;
    }
    
    Buffer server_game = recv_all(socket_fd);
    if (server_game.length == -1) {
        perror("recv_all(socket_fd)");
        check_perror(close(socket_fd));
        return -1;
    }
    
    // deserialization sets default values for a client
    Game_deserialize(game, &server_game);
    free(server_game.data);
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
    GameClient *const client = (GameClient *) realloc(game->client, sizeof(GameClient));
    if (!client) {
        perror("Malloc Failed: realloc(game->client, sizeof(GameClient))");
        return -1;
    }
    
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        sdl_perror("SDL_Init(SDL_INIT_EVERYTHING)");
        if (str_contains(SDL_GetError(), "No available video device")) {
            sdl_log_func(stderr, SDL_GetNumVideoDrivers(), 1);
            sdl_log_func(stderr, SDL_GetNumVideoDisplays(), 1);
        }
        return -1;
    }
    
    if (GameClient_init(client, game) == -1) {
        sdl_perror("GameClient_init(game->client, game)");
        goto error;
    }
    
    if (GameState_load_sprites(&game->state, client->renderer) == -1) {
        perror("GameState_load_sprites(&game->state)");
        goto error;
    }
    
    set_field(game->client, client);
    return 0;
    
    error:
    GameClient_destroy(client);
    SDL_Quit();
    return -1;
}

static int Game_client_add_own_player_with_name_and_texture(
        Game *const game, const int socket_fd, const String name, const GameTexture texture) {
    Player *const own_player = Player_new(name, texture);
    
    Buffer serialized_player = Player_serialize(own_player);
    
    if (send_all(socket_fd, serialized_player) == -1) {
        perror("send_all(socket_fd, serialized_player)");
        goto error;
    }
    free(serialized_player.data);
    serialized_player = {0};
    
    if (Game_add_player(game, own_player) == -1) {
        perror("Game_add_player(game, own_player)");
        goto error;
    }
    
    set_field(own_player->socket_fd, socket_fd);
    set_field(game->state.own_player, own_player);
    return 0;
    
    error:
    free(own_player);
    free(serialized_player.data);
    return -1;
}

#define define_get_player_field(field, Type, invalid_expr, invalid_value) \
static Type get_player_##field(const Players *const players) { \
    for (uint8_t attempt = 0; attempt < MAX_PLAYER_CHOOSES; ++attempt) { \
        const Type value = choose_player_##field(); \
        if (invalid_expr) { \
            return invalid_value; \
        } \
         \
        if (!Players_have_##field(players, value)) { \
            return value; \
        } \
    } \
    perror("exceeded max num attempts to choose player "#field); \
    return invalid_value; \
}

define_get_player_field(name, String, !value.chars, INVALID_STRING);

define_get_player_field(texture, GameTexture, value == INVALID_PLAYER, INVALID_PLAYER);

#undef define_get_player_field

static int Game_client_add_own_player(Game *const game, const int socket_fd) {
    const Players *const players = game->state.players;
    
    const String name = get_player_name(players);
    if (!name.chars) {
        perror("Invalid Name: get_player_name(players)");
        goto error;
    }
    
    const GameTexture texture = get_player_texture(players);
    if (texture == INVALID_PLAYER) {
        perror("Invalid Player Texture: get_player_texture(players)");
        goto error;
    }
    
    if (Game_client_add_own_player_with_name_and_texture(game, socket_fd, name, texture) == -1) {
        perror("Game_client_add_own_player_with_name_and_texture(game, socket_fd, name, texture)");
        goto error;
    }
    return 0;
    
    error:
    free(name.chars);
    return -1;
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