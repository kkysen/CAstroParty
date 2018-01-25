#include "networking.h"
#include "textures.h"
#include <SDL2/SDL.h>
#include <stdbool.h>

#ifndef D_PLAYER
#define D_PLAYER

struct player {
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
    
    float angle;
    float acceleration;
    int server_index;

    // Keyboard inputs
    bool button_turn;
    bool button_shoot;
    // Whether we pressed the shoot key last time
    bool button_shoot_prev;
    
    float vel_angle;
    
    SDL_Rect *rect; // For visual purposes only
    
    const Sprite *sprite;
};

struct player *Player_create(float x, float y, int server_index);
void Player_update(struct player *player);
void Player_render(struct player *player);

void Player_update_keys(struct player *player, struct Networking_unpacked_inputs);

#endif
