
#include <SDL2/SDL.h>
#include "vector.h"

#ifndef D_BULLET
#define D_BULLET

// Number of frames before a bullet gets deleted
#define BULLET_TIMER_TIMEOUT 180

typedef struct {
    union {
        struct {
            float x;
            float y;
        };
        Vector pos;
    };
    union {
        struct {
            float vel_x;
            float vel_y;
        };
        Vector vel;
    };
    
    int timer; // Timer, increasing each frame
    
    SDL_Rect *rect; // Drawing purposes only
} Bullet;


Bullet *Bullet_create(Vector pos, float angle);

void Bullet_update(Bullet *bullet);

void Bullet_render(Bullet *bullet);

#endif
