//
// Created by kkyse on 12/24/2017.
//

#include "game2.h"

#include <pthread.h>

#include "util/sdl_colors.h"
#include "util/sdl_utils.h"
#include "util/hash.h"
#include "networking.h"

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

// Game must be declared on heap for multi-threading to work
Game *_Game_new(bool is_client) {
    Game *const game = (Game *) malloc(sizeof(Game));
    if (!game) {
        perror("malloc(sizeof(Game))");
        return NULL;
    }
    if (_Game_init_default(game, is_client) == -1) {
        sdl_perror("Game_init_default(game, is_client)");
        free(game);
        return NULL;
    }
    return game;
}

// Game must be declared on heap for multi-threading to work
int _Game_init_default(Game *game, bool is_client) {
    sdl_check_perror(_Game_init(
            game, is_client, GAME_TITLE,
            WINDOW_WIDTH, WINDOW_HEIGHT, GAME_FPS,
            GAME_DEFAULT_NUM_PLAYERS));
    return 0;
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

int _Game_init(Game *game, bool is_client, const char *title,
               int width, int height, uint8_t fps, uint8_t max_num_players) {
    const bool render = is_client; // this should always be true;
    // Game::is_client and Game::render are const, so memcpy
    memcpy((bool *) &game->is_client, &is_client, sizeof(bool));
    memcpy((bool *) &game->render, &render, sizeof(bool));
    
    game->title = str_copy(title);
    
    game->prev_time = 0;
    game->interrupt = NULL;
    game->is_running = false;
    game->quit = false;
    
    Players *const players = (Players *) malloc(sizeof(Players));
    check_null_msg(players, "malloc(sizeof(Players))");
    
    players->max_num_players = max_num_players;
    players->num_players = 0;
    players->players = NULL;
    
    Players_add(players, own_player);
    
    const GameState state = {
            ._x = 0,
            ._y = 0,
            .width = width,
            .height = height,
            .size = Vector_new(width, height),
            .players = players,
            .own_player = is_client ? NULL : own_player,
    };
    memcpy(&game->state, &state, sizeof(GameState));
    
    // must write these after memcpy of GameState overwrites them
    game->fps = fps;
    game->tick = 0;
    
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
    const uint8_t fps = game->fps;
    const uint64_t tick = game->tick;
    // will overwrite unionized fps and tick, so must restore these fields
    // b/c fps and tick might be useful after game ended
    const GameState state = {0};
    memcpy(&game->state, &state, sizeof(GameState));
    game->fps = fps;
    game->tick = tick;
    
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
    
    free((char *) game->title);
    game->title = NULL;
}

int Game_add_player(const Game *const game, Player *const player) {
//    player->sprite = *get_sprite(player->sprite.id, game->renderer);
    memcpy(&player->sprite, get_sprite(player->sprite.id, game->renderer), sizeof(Sprite));
    check_perror(Players_add(game->state.players, player));
    printf("%s's sprite: %s (%dx%d)\n",
           player->name,
           get_texture_name(player->sprite.id),
           player->sprite.width,
           player->sprite.height);
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
            printf("Quitting Game \"%s\"\n", game->title);
            Game_destroy(game);
            return;
        }
        Game_loop(game);
    }
}

static void Game_loop(Game *const game) {
    game->tick++;
    
    // apparently V-Sync already restricts FPS to an optimal rate for the computer
    const uint64_t current_time = SDL_GetPerformanceCounter();
    GameState *const state = &game->state;
    SDL_Renderer *const renderer = game->renderer;
    if (game->is_running) {
        const uint64_t delta_cycles = current_time - game->prev_time;
        const float delta_time = (delta_cycles * 1000.0f) / SDL_GetPerformanceFrequency();
        GameState_update(state, delta_time);
        if (game->render) {
            GameState_render(state, renderer);
        }
    }
    game->prev_time = current_time;
}

static void GameState_update(GameState *const state, const float delta_time) {
    // this should only be updated once here
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
    _hash(state->fps);
    _hash(state->tick);
    _hash(state->width);
    _hash(state->height);
    _hash(Player_hash((const Player *) state->players));
    return current_hash;
}

uint64_t Game_hash(const Game *const game) {
    uint64_t current_hash = PRIME_64;
    _hash(game->is_running);
    _hash(game->quit);
    _hash(game->fps);
    _hash(game->tick);
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

typedef struct {
    Game *const game;
    const double seconds;
} TimedGame;

static void *_Game_quit_later(void *arg) {
    const TimedGame *const timed_game = (TimedGame *) arg;
    printf("Will quit Game \"%s\" in %f seconds\n", timed_game->game->title, timed_game->seconds);
    double_sleep(timed_game->seconds);
    Game_quit(timed_game->game);
    free((void *) timed_game);
    return NULL;
}

// Game must be allocated on heap for this to work,
// because it uses a separated thread and threads have separate stacks
void Game_quit_later(Game *const game, const double seconds) {
    const TimedGame timed_game = {.game = game, .seconds = seconds};
    TimedGame *const timed_game_ptr = (TimedGame *) malloc(sizeof(TimedGame));
    memcpy(timed_game_ptr, &timed_game, sizeof(TimedGame));
    
    pthread_t thid;
    if (pthread_create(&thid, NULL, _Game_quit_later, (void *) timed_game_ptr) != 0) {
        perror("pthread_create(&thid, NULL, _Game_quit_later, (void *) &timed_game)");
    }
}