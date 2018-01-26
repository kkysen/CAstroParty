
#include <SDL2/SDL.h>
#include "vector.h"
#include "textures.h"

#ifndef D_BULLET
#define D_BULLET

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

    int player_server_index; // The server_id of the player that shot it

    const Sprite sprite;
} Bullet;


Bullet *Bullet_create(Vector position, float angle);

void Bullet_update(Bullet *bullet);

void Bullet_render(Bullet *bullet);

#endif
