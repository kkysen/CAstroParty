#include "networking.h"
#include "textures.h"
#include <SDL2/SDL.h>
#include <stdbool.h>

#ifndef D_PLAYER
#define D_PLAYER

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
    
    float acceleration;
    
    float angle;
    float angular_velocity;
    
    const size_t server_index;
    
    // Keyboard inputs
    bool button_turn;
    bool button_shoot;
    // Whether we pressed the shoot key last time
    bool button_shoot_prev;
    
    Sprite sprite;
    
    bool alive;
} Player;

Player *Player_create(Vector position, size_t server_index);

void Player_update(Player *player);

void Player_render(Player *player);

void Player_update_keys(Player *player, struct Networking_unpacked_inputs);

Vector Player_direction(Player *player);

#endif
