#include "networking.h"
#include <SDL2/SDL.h>
#include <stdbool.h>

#ifndef D_PLAYER
#define D_PLAYER

struct player {
    float x; float y; float angle;
    int server_index;

    // Keyboard inputs
    bool key_accelerate;
    bool key_turn_left;
    bool key_shoot;
    // Whether we pressed the shoot key last time
    bool key_shoot_prev;

    float vel_x; float vel_y;
    float vel_angle;
    
    SDL_Rect *rect;
};

struct player *Player_create(float x, float y, int server_index);
void Player_update(struct player *player);
void Player_render(struct player *player);

void Player_update_keys(struct player *player, struct Networking_unpacked_inputs);

#endif
