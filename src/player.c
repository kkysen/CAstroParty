//
// Created by kkyse on 12/24/2017.
//

#include "player.h"

Player *Player_new(const char *const name, const SDL_Color color) {
    const Player local_player = (Player) {
            .id = 0;
            .name = name,
            .color = color,
            .position = {0, 0},
            .velocity = {0, 0},
            .orientation = 0,
            .ammo = 0
    };
    Player *const player = (Player *) malloc(sizeof(Player));
    memcpy(player, &local_player, sizeof(Player));
    return player;
}

void Player_spawn(Player *const player, const Vector position, const float orientation, const char ammo) {
    player->position = position;
    player->orientation = orientation;
    player->ammo = ammo;
    // TODO
}

static inline Vector Player_get_acceleration(const Player *const player) {
    const float orientation_radians = deg2rad(player->orientation);
    return (Vector) {
            .x = ACCELERATION * cosf(orientation_radians),
            .y = ACCELERATION * sinf(orientation_radians)
    };
}

bool Player_in_radius(const Player *const player, Vector position, const float radius) {
    return Vector_in_radius(player->position, position, radius);
}

void Player_update(Player *const player, const float delta_time) {
    // TODO incorporate delta_time
    const Vector acceleration = Player_get_acceleration(player);
    Vector_i_add(player->velocity, acceleration);
    Vector_i_scale(player->velocity, MAX_SPEED_FACTOR);
    Vector_i_add(player->position, player->velocity);
}

void Player_render(const Player *const player, SDL_Renderer *const renderer) {
    // TODO
}