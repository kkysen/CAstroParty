#ifndef CASTROPARTY_PLAYER_H
#define CASTROPARTY_PLAYER_H

#include <stdbool.h>
#include <SDL2/SDL.h>

#include "vector.h"
#include "textures.h"
#include "util/sized_string.h"

#define ACCELERATION 0.166666666666f
#define ANGULAR_VELOCITY 6
#define MAX_SPEED_FACTOR 0.6666666666666f

typedef struct {
    uint8_t id;
    const String name;
    Sprite sprite;
    Vector position;
    Vector velocity;
    float orientation;
    char ammo; // TODO why is this a char?
    const bool is_own;
    const int socket_fd;
} Player;

#include "game.h"

Player *Player_new(String name, GameTexture texture);

/**
 * Place player in the world.
 *
 * @param player
 * @param position
 * @param orientation
 * @param ammo
 */
void Player_spawn(Player *player, Vector position, float orientation, char ammo);

bool Player_in_radius(const Player *player, Vector position, float radius);

void Player_update(Player *player, GameState *state, float delta_time);

void Player_render(const Player *player, const GameState *state, SDL_Renderer *renderer);

uint64_t Player_hash(const Player *player);

#endif // CASTROPARTY_PLAYER_H
