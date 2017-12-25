//
// Created by kkyse on 12/24/2017.
//

#include "game.h"

#include "util/sdl_colors.h"
#include "util/sdl_utils.h"
#include "util/string_utils.h"

static int Game_init_renderer(Game *const game) {
    game->renderer = SDL_CreateRenderer(game->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    sdl_check_null_perror_msg(game->renderer, "SDL_CreateRenderer(game->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC)");
    return 0;
}

static int Game_init_window(Game *const game) {
    game->window = SDL_CreateWindow(
            game->title,
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            game->width,
            game->height,
            SDL_WINDOW_OPENGL
    );
    sdl_check_null_perror_msg(game->window, "SDL_CreateWindow( game->title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, game->width, game->height, SDL_WINDOW_OPENGL)");
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

int Game_init(Game *const game, const char *const title,
              const int width, const int height, const uint8_t fps, const uint8_t max_num_players) {
    game->title = str_copy(title);
    game->width = width;
    game->height = height;
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
    game->players = players;
    
    sdl_check_perror(SDL_Init(SDL_INIT_EVERYTHING));
    if (Game_init_window_and_renderer(game) == -1) {
        SDL_Quit();
        free(game->players);
        sdl_perror("Game_init_window_and_renderer(game)");
        return -1;
    }
    
    return 0;
}

void Game_quit(Game *const game) {
    Players_free(game->players);
    free(game->players);
    game->players = NULL;
    
    SDL_DestroyRenderer(game->renderer);
    game->renderer = NULL;
    
    SDL_DestroyWindow(game->window);
    game->window = NULL;
    
    SDL_Quit();
    
    game->quit = true;
    game->is_running = false;
    
    game->interrupt = NULL;
    
    game->fps = 0;
    
    game->height = 0;
    game->width = 0;
    
    free((char *) game->title);
    game->title = NULL;
}

int Game_add_player(const Game *const game, Player *const player) {
    check_perror(Players_add(game->players, player));
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
            Game_quit(game);
            return;
        }
        Game_loop(game);
    }
}

void Game_loop(Game *const game) {
    // apparently V-Sync already restricts FPS to an optimal rate for the computer
    const uint64_t current_time = SDL_GetPerformanceCounter();
    if (game->is_running) {
        const uint64_t delta_ticks = current_time - game->prev_time;
        const float delta_time = (delta_ticks * 1000.0f) / SDL_GetPerformanceFrequency();
        Game_update(game, delta_time);
        Game_render(game);
    }
    game->prev_time = current_time;
}

void Game_update(Game *const game, const float delta_time) {
    Players_update(game->players, delta_time);
}

void Game_render(const Game *const game) {
    SDL_SetRenderDrawColor(game->renderer, rgba_args(BLACK));
    SDL_RenderClear(game->renderer);
    
    Players_render(game->players, game->renderer);
    
    SDL_RenderPresent(game->renderer);
}