#include<SDL2/SDL.h>


#ifndef D_GAME
#define D_GAME

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480
#define GAME_TITLE "C-Astro Party"

#define GAME_FPS 60

// NetworkMode. Are we the server, or a client?
enum NetworkMode { NETWORK_MODE_CLIENT, NETWORK_MODE_SERVER };

SDL_Window *Game_window;
SDL_Renderer *Game_renderer;

enum NetworkMode Game_network_mode;

void Game_stop();

#endif
