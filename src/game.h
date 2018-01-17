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
};

typedef void (*GameInterruptor)(Game *const game);

struct game {
    const bool render;
    const char *title;
    uint64_t prev_time;
    volatile GameInterruptor interrupt;
    volatile bool is_running;
    volatile bool quit;
    SDL_Window *window;
    SDL_Renderer *renderer;
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

Game *Game_new(bool render);

// TODO change width and height to doubles that are a fraction of the screen's total size
int Game_init(Game *game, bool render,
              const char *title, int width, int height,
              uint8_t fps, uint8_t max_num_players);

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
