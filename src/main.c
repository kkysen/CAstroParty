#include <SDL2/SDL.h>

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480
#define GAME_TITLE "C-raft"

#define GAME_FPS 60

// TODO: Move headers to a header file and figure out how to import those
void init_sdl();
void quit_sdl();

// Main loop functions
void main_loop();
void main_tick();
void main_render();

SDL_Window *window;
SDL_Renderer *renderer;

int game_running = 0;

/* init_sdl
 *
 * Initializes our sdl components
 *
 */
void init_sdl() {
    if (SDL_Init( SDL_INIT_EVERYTHING ) != 0) {
        printf("SDL Init failed! You're kinda screwed now\n");
        exit(1);
    }

    window = SDL_CreateWindow(
            GAME_TITLE, 
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            WINDOW_WIDTH,
            WINDOW_HEIGHT,
            SDL_WINDOW_OPENGL//SDL_WINDOW_BORDERLESS//0 //SDL_WINDOW_INPUT_GRABBED
            );
    if (window == NULL) {
        printf("SDL Window failed to create. Have fun\n");
        exit(1);
    }

    renderer = SDL_CreateRenderer(
            window, 
            -1, 
            SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
            );
    if (renderer == NULL) {
        SDL_DestroyWindow(window);
        printf("SDL Renderer failed to create. Good luck working with an empty window\n");
        SDL_Quit();
        exit(1);
    }
}

/* quit_sdl
 *
 * De-initializes and discards our sdl components
 *
 */
void quit_sdl() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void main_tick() {
    printf("Tick\n");
}

void main_render() {
    printf("Render\n");
}

void main_loop() {
    game_running = 1;
    unsigned int prev_time = SDL_GetTicks();
    unsigned int delta_time;

    int should_render = 0;

    unsigned int ms_per_frame = 1000 * GAME_FPS;
    while(game_running) {
        should_render = 0;

        delta_time = SDL_GetTicks() - prev_time;
        while(delta_time > ms_per_frame) {
            delta_time -= ms_per_frame;
            should_render = 1;

            main_tick();

            prev_time = SDL_GetTicks();
        }

        if (should_render) {
            main_render();
        }

    }
}

int main() {

    //init_sdl();

    main_loop();

    //quit_sdl();

    return 0;

    /*SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(
            renderer,
            255,
            0,
            0,
            255);
    
    SDL_Rect rect = {.x = 10, .y = 10, .w = 100, .h = 100};

    SDL_RenderDrawRect(renderer, &rect);
    
    SDL_RenderPresent(renderer);

    SDL_Delay(1500);
    */
}

