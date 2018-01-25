
#include <SDL2/SDL.h>

#ifndef D_BULLET
#define D_BULLET

// Number of frames before a bullet gets deleted
#define BULLET_TIMER_TIMEOUT 180

struct bullet {
    float x; float y;
    float vel_x; float vel_y;
    
    int timer; // Timer, increasing each frame

    SDL_Rect *rect; // Drawing purposes only
};


struct bullet *Bullet_create(float x, float y);
void Bullet_update(struct bullet *bullet);
void Bullet_render(struct bullet *bullet);

#endif
