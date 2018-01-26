
#ifndef D_BULLET
#define D_BULLET

#include <SDL2/SDL.h>
#include "vector.h"
#include "textures.h"

// Number of frames before a bullet gets deleted
#define BULLET_TIMER_TIMEOUT 180

typedef struct {
    union {
        Vector position;
        struct {
            float x;
            float y;
        };
    };
    union {
        Vector velocity;
        struct {
            float vel_x;
            float vel_y;
        };
    };
    
    int timer; // Timer, increasing each frame
    
    const Sprite sprite;
    
    const size_t player_server_index;
} Bullet;


Bullet *Bullet_create(Vector position, float angle, size_t player_server_index);

void Bullet_update(Bullet *bullet);

void Bullet_render(Bullet *bullet);

#endif
