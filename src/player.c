//
// Created by kkyse on 12/24/2017.
//

#include "player.h"

#include "util/hash.h"
#include "util/sdl_utils.h"
#include "textures.h"

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
    
    if (false) {
        player->orientation += 360.0f / 60.0f / 5.0f; //ANGULAR_VELOCITY;
    }
    
    // only true if button pressed and is currently accelerating
    if (false) {
        const Vector acceleration = Player_get_acceleration(player);
        Vector_i_mul_add(velocity, acceleration, delta_time);
    }
    
    Vector_i_scale(velocity, MAX_SPEED_FACTOR);
    Vector_i_mul_add(position, velocity, delta_time);
    
    // keep in borders
    
    Vector center = player->sprite.center;
    
    // performant, possibly branchless if optimized
    Vector_clamp(position,
                 center.x, center.y,
                 state->width - center.x, state->width - center.y);
    
    // copy back to Player*
    player->position = position;
    player->velocity = velocity;
}

void Player_render(const Player *const player, const GameState *const state, SDL_Renderer *const renderer) {
//    printf("Rendering player: \"%s\"\n", player->name);
    if (state->tick % state->fps == 0) {
        // run once a second
        pz(state->tick);
        pv(player->position);
        pv(player->velocity);
    }
    const Vector position = player->position;
    const Vector sprite_center = player->sprite.center;
    const SDL_Rect dest_rect = {
            .x = (int) (position.x - sprite_center.x),
            .y = (int) (position.y - sprite_center.y),
            .w = (int) sprite_center.x,
            .h = (int) sprite_center.y,
    };
    SDL_Point center = Vector_as_SDL_Point(position);
    sdl_warn_perror(SDL_RenderCopyEx(
            renderer,
            player->sprite.texture,
            NULL, // TODO change later
            &dest_rect,
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
