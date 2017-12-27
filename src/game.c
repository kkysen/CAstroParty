//
// Created by kkyse on 12/24/2017.
//

#include "game.h"

#include "util/sdl_colors.h"
#include "util/sdl_utils.h"
#include "util/string_utils.h"
#include "util/hash.h"
#include "player.h"

static int Game_init_renderer(Game *game);

static int Game_init_window(Game *game);

static int Game_init_window_and_renderer(Game *game);

static void GameState_free(GameState *state);

static void Game_loop(Game *game);

static void GameState_update(GameState *state, float delta_time);

static void GameState_render(const GameState *state, SDL_Renderer *renderer);

static uint64_t GameState_hash(const GameState *state);

static void _GameInterruptor_pause(Game *game);

static void _GameInterruptor_quit(Game *game);

// =============================================

Game *Game_new(const bool render) {
    Game *const game = (Game *) malloc(sizeof(Game));
    if (!game) {
        perror("malloc(sizeof(Game))");
        return NULL;
    }
    if (Game_init(
            game, render, GAME_TITLE,
            WINDOW_WIDTH, WINDOW_HEIGHT, GAME_FPS,
            GAME_DEFAULT_NUM_PLAYERS) == -1) {
        sdl_perror("Game_init(game, render, GAME_TITLE, WINDOW_WIDTH, WINDOW_HEIGHT, GAME_FPS, GAME_DEFAULT_NUM_PLAYERS)");
        free(game);
        return NULL;
    }
    return game;
}

static int Game_init_renderer(Game *const game) {
    game->renderer = SDL_CreateRenderer(game->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    sdl_check_null_perror_msg(game->renderer,
                              "SDL_CreateRenderer(game->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC)");
    return 0;
}

static int Game_init_window(Game *const game) {
    game->window = SDL_CreateWindow(
            game->title,
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            game->state.width,
            game->state.height,
            SDL_WINDOW_OPENGL
    );
    sdl_check_null_perror_msg(game->window,
                              "SDL_CreateWindow( game->title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, game->width, game->height, SDL_WINDOW_OPENGL)");
    
    return 0;
}

static int Game_init_window_and_renderer(Game *const game) {
    sdl_check_perror(Game_init_window(game));
    if (Game_init_renderer(game) == -1) {
        SDL_DestroyWindow(game->window);
        sdl_perror("Game_init_renderer(game)");
        return -1;
    }
    return 0;
}

int Game_init(Game *const game, const bool render, const char *const title,
              const int width, const int height, const uint8_t fps, const uint8_t max_num_players) {
    // Game.render is const, so memcpy
    memcpy((bool *) &game->render, &render, sizeof(bool));
    
    game->title = str_copy(title);
    game->fps = fps;
    
    game->prev_time = 0;
    game->interrupt = NULL;
    game->is_running = false;
    game->quit = false;
    
    Players *const players = (Players *) malloc(sizeof(Players));
    check_null_msg(players, "malloc(sizeof(Players))");
    
    players->max_num_players = max_num_players;
    players->num_players = 0;
    players->players = NULL;
    
    const GameState state = {
            .width = width,
            .height = height,
            .players = players,
    };
    memcpy(&game->state, &state, sizeof(GameState));
//    game->state = state;
    
    if (game->render) {
        if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
            sdl_perror("SDL_Init(SDL_INIT_EVERYTHING)");
            if (str_contains(SDL_GetError(), "No available video device")) {
                sdl_log_func(stderr, SDL_GetNumVideoDrivers(), 1);
                sdl_log_func(stderr, SDL_GetNumVideoDisplays(), 1);
            }
            return -1;
        }
        
        if (Game_init_window_and_renderer(game) == -1) {
            sdl_perror("Game_init_window_and_renderer(game)");
            SDL_Quit();
            free(game->state.players);
            return -1;
        }
    }
    
    return 0;
}

static void GameState_free(GameState *const state) {
    Players_free(state->players);
    free(state->players);
}

// TODO must also signal to other games on network
void Game_destroy(Game *const game) {
    GameState_free(&game->state);
    const GameState state = {
            .width = 0,
            .height = 0,
            .players = NULL,
    };
    memcpy(&game->state, &state, sizeof(GameState));
//    game->state = state;
    
    if (game->render) {
        SDL_DestroyRenderer(game->renderer);
        game->renderer = NULL;
    
        SDL_DestroyWindow(game->window);
        game->window = NULL;
    
        SDL_Quit();
    }
    
    game->quit = true;
    game->is_running = false;
    
    game->interrupt = NULL;
    
    game->fps = 0;
    
    free((char *) game->title);
    game->title = NULL;
}

int Game_add_player(const Game *const game, Player *const player) {
    check_perror(Players_add(game->state.players, player));
    memcpy(&player->sprite, get_sprite(player->sprite.id, game->renderer), sizeof(Sprite));
//    player->sprite = *get_sprite(player->sprite.id, game->renderer);
    // TODO if (!game->render) don't load texture, but still want texture size
    return 0;
}

void Game_run(Game *const game) {
    game->is_running = true;
    game->prev_time = SDL_GetPerformanceCounter();
    
    for (;;) {
        if (game->interrupt) {
            // allow for other threads to set an interrupt function
            // that can pause (is_running = false), quit, or etc. the game
            game->interrupt(game);
        }
        if (game->quit) {
            Game_destroy(game);
            return;
        }
        Game_loop(game);
    }
}

static void Game_loop(Game *const game) {
    // apparently V-Sync already restricts FPS to an optimal rate for the computer
    const uint64_t current_time = SDL_GetPerformanceCounter();
    GameState *const state = &game->state;
    SDL_Renderer *const renderer = game->renderer;
    if (game->is_running) {
        const uint64_t delta_ticks = current_time - game->prev_time;
        const float delta_time = (delta_ticks * 1000.0f) / SDL_GetPerformanceFrequency();
        GameState_update(state, delta_time);
        if (game->render) {
            GameState_render(state, renderer);
        }
    }
    game->prev_time = current_time;
}

static void GameState_update(GameState *const state, const float delta_time) {
    Players_update(state->players, state, delta_time);
}

static void GameState_render(const GameState *const state, SDL_Renderer *const renderer) {
    SDL_SetRenderDrawColor(renderer, rgba_args(BLACK));
    SDL_RenderClear(renderer);
    
    Players_render(state->players, state, renderer);
    
    SDL_RenderPresent(renderer);
}

#define _hash(val) hash(current_hash, (double) (val))

static uint64_t GameState_hash(const GameState *const state) {
    uint64_t current_hash = PRIME_64;
    _hash(state->width);
    _hash(state->height);
    _hash(Player_hash((const Player *) state->players));
    return current_hash;
}

uint64_t Game_hash(const Game *const game) {
    uint64_t current_hash = PRIME_64;
    _hash(game->is_running);
    _hash(game->fps);
    _hash(fnv1a_64_hash(game->title));
    _hash(GameState_hash(&game->state));
    return current_hash;
}

#undef _hash

static void _GameInterruptor_pause(Game *const game) {
    game->is_running = false;
}

const GameInterruptor GameInterruptor_pause = _GameInterruptor_pause;

void Game_pause(Game *const game) {
    game->interrupt = GameInterruptor_pause;
}

static void _GameInterruptor_quit(Game *const game) {
    game->quit = true;
}

const GameInterruptor GameInterruptor_quit = _GameInterruptor_quit;

void Game_quit(Game *const game) {
    game->interrupt = GameInterruptor_quit;
}
