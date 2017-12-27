//
// Created by kkyse on 12/24/2017.
//

#ifndef CASTROPARTY_GAME_H
#define CASTROPARTY_GAME_H

#include <stdbool.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_render.h>

#include "player.h"
#include "players.h"

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480
#define GAME_TITLE "C-Astro Party"

#define GAME_FPS 60
#define GAME_DEFAULT_NUM_PLAYERS 4

typedef struct game Game;

typedef void (*GameInterruptor)(Game *const game);

struct game {
    const char *title;
    int width;
    int height;
    uint8_t fps;
    uint64_t prev_time;
    volatile GameInterruptor interrupt;
    volatile bool is_running;
    volatile bool quit;
    SDL_Window *window;
    SDL_Renderer *renderer;
    Players *players;
};

Game *Game_new();

int Game_init(Game *game, const char *title, int width, int height, uint8_t fps, uint8_t max_num_players);

void Game_destroy(Game *game);

int Game_add_player(const Game *game, Player *player);

void Game_run(Game *game);

void Game_loop(Game *game);

void Game_update(Game *game, float delta_time);

void Game_render(const Game *game);

uint64_t Game_hash(const Game *game);

extern const GameInterruptor GameInterruptor_pause;

void Game_pause(Game *game);

extern const GameInterruptor GameInterruptor_quit;

void Game_quit(Game *game);

#endif // CASTROPARTY_GAME_H
