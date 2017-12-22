#include <SDL2/SDL.h>

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

int main() {

    if (SDL_Init( SDL_INIT_EVERYTHING ) != 0) {
        printf("SDL Init failed! You're kinda screwed now\n");
        exit(1);
    }

    SDL_Window *window = SDL_CreateWindow(
            "Hoi walrd!", 
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            WINDOW_WIDTH,
            WINDOW_HEIGHT,
            0//SDL_WINDOW_BORDERLESS//0 //SDL_WINDOW_INPUT_GRABBED
            );
    if (window == NULL) {
        printf("SDL Window failed to create. Have fun\n");
        exit(1);
    }

    SDL_Renderer *render = SDL_CreateRenderer(
            window, 
            -1, 
            SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
            );
    if (render == NULL) {
        SDL_DestroyWindow(window);
        printf("SDL Renderer failed to create. Good luck working with an empty window\n");
        SDL_Quit();
        exit(1);
    }

    SDL_RenderClear(render);

    /*TODO: This breaks everytang SDL_SetRenderDrawColor(
            render,
            255,
            0,
            255,
            255);
    */

    SDL_Rect rect = {.x = 10, .y = 10, .w = 100, .h = 100};

    SDL_RenderDrawRect(render, &rect);
    
    SDL_RenderPresent(render);

    SDL_Delay(1500);


    SDL_DestroyRenderer(render);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

