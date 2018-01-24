//
// Created by kkyse on 12/24/2017.
//

#ifndef CASTROPARTY_GAME_H
#define CASTROPARTY_GAME_H

#include <stdbool.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_render.h>

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480
#define GAME_TITLE "C Astro Party"

#define GAME_FPS 60
#define GAME_DEFAULT_NUM_PLAYERS 4

typedef struct game_state GameState;

typedef struct game Game;

#include "player.h"
#include "players.h"
#include "util/sized_string.h"

typedef struct {
    const int listening_socket_fd;
    pthread_t const listening_thread; // pthread_t is a typedef'd pointer // NOLINT
} GameServer;

typedef struct {
    SDL_Window *const window;
    SDL_Renderer *const renderer;
} GameClient;

struct game_state {
    // mutable only by overlapping (union-ized) members in Game
    const uint8_t fps;
    const uint64_t tick;
    union {
        struct {
            int _x;
            int _y;
            const int width;
            const int height;
        };
        const SDL_Rect rect;
    };
    const Vector size;
    Players *const players;
    Player *const own_player;
};

typedef void (*GameInterruptor)(Game *const game);

struct game {
    union {
        const bool is_client;
        const bool render;
    };
    union {
        GameServer *const server;
        GameClient *const client;
    };
    const String title;
    uint64_t prev_time;
    volatile GameInterruptor interrupt;
    volatile bool is_running;
    volatile bool quit;
    union {
        // struct provides mutable accessors to
        // fps and tick members of GameStates
        struct {
            uint8_t fps;
            uint64_t tick;
        };
        GameState state;
    };
};

// @Deprecated
Game *_Game_new(bool is_client);

// @Deprecated
int _Game_init_default(Game *game, bool is_client);

// @Deprecated
// TODO change width and height to doubles that are a fraction of the screen's total size
int _Game_init(Game *game, bool is_client,
               const char *title, int width, int height,
               uint8_t fps, uint8_t max_num_players);

int Game_init_client(Game *game);

int Game_init_server(
        Game *game,
        const char *title,
        uint8_t fps,
        uint8_t max_num_players,
        Vector relative_texture_size,
        uint16_t listening_port,
        const char *ip_address);

void Game_destroy(Game *game);

int Game_add_player(const Game *game, Player *player);

void Game_run(Game *game);

uint64_t Game_hash(const Game *game);

extern const GameInterruptor GameInterruptor_pause;

void Game_pause(Game *game);

extern const GameInterruptor GameInterruptor_quit;

void Game_quit(Game *game);

void Game_quit_later(Game *game, double seconds);

#endif // CASTROPARTY_GAME_H