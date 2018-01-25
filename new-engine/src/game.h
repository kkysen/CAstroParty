#include<SDL2/SDL.h>
#include "vector.h"


#ifndef D_GAME
#define D_GAME

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720
#define GAME_TITLE "C-Astro Party"

#define GAME_FPS 60

extern uint64_t game_tick;

extern const Vector window_size;

// NetworkMode. Are we the server, or a client?
enum NetworkMode { NETWORK_MODE_CLIENT, NETWORK_MODE_SERVER };

SDL_Window *Game_window;
SDL_Renderer *Game_renderer;

enum NetworkMode Game_network_mode;

void Game_stop();

#endif
