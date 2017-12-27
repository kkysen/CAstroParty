//
// Created by kkyse on 12/24/2017.
//

#include "player.h"

#include "util/hash.h"
#include "util/sdl_utils.h"

Player *Player_new(const char *const name, const GameTexture texture) {
    Player *const player = (Player *) malloc(sizeof(Player));
    if (!player) {
        return NULL;
    }
    // Player.id:  will be set later when added to a Players in a Game
    // Player.sprite: only id set for now,
    //                Game_add_player() will get actual Sprite using Game.renderer and id
    const Player local_player = (Player) {
            .id = 0,
            .name = name, // TODO should this be copied and freed on Player_free()?
            .sprite = {.id = texture, NULL, 0, 0},
            .position = Vector_new(0, 0),
            .velocity = Vector_new(0, 0),
            .orientation = 0,
            .ammo = 0
    };
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

void Player_update(Player *const player, GameState *const state, const float delta_time) {
    // copy to stack for faster access
    Vector position = player->position;
    Vector velocity = player->velocity;
    const Vector acceleration = Player_get_acceleration(player);
    Vector_i_mul_add(velocity, acceleration, delta_time);
    Vector_i_scale(velocity, MAX_SPEED_FACTOR);
    Vector_i_mul_add(position, velocity, delta_time);
    
    // keep in borders
    if (position.x < 0) {
        position.x = 0;
    } else if (position.x > state->width) {
        position.x = state->width;
    }
    
    if (position.y < 0) {
        position.y = 0;
    } else if (position.y > state->height) {
        position.y = state->height;
    }
    
    // copy back to Player*
    player->position = position;
    player->velocity = velocity;
}

void Player_render(const Player *const player, const GameState *const state, SDL_Renderer *const renderer) {
    printf("Rendering player: \"%s\"\n", player->name);
    const SDL_Point center = Vector_as_SDL_Point(player->position);
    sdl_warn_perror(SDL_RenderCopyEx(
            renderer,
            player->sprite.texture,
            NULL, // TODO change later
            NULL, // TODO change later
            player->orientation,
            &center,
            SDL_FLIP_NONE
    ));
}

#define _hash(val) hash(current_hash, val)

uint64_t Player_hash(const Player *const player) {
    uint64_t current_hash = PRIME_64;
    _hash(player->id);
    _hash(player->position.bits);
    _hash(player->velocity.bits);
    _hash(player->orientation);
    _hash(player->ammo);
    _hash(player->sprite.id);
    _hash(fnv1a_64_hash(player->name));
    return current_hash;
}

#undef _hash
