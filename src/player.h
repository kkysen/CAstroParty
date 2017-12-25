#ifndef CASTROPARTY_PLAYER_H
#define CASTROPARTY_PLAYER_H

#include <string.h>
#include <math.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

#include "vector.h"

#define ACCELERATION 0.166666666666f
#define ANGULAR_VELOCITY 6
#define MAX_SPEED_FACTOR 0.6666666666666f

typedef struct {
    uint8_t id;
    const char *const name;
	SDL_Color color;
	Vector position;
	Vector velocity;
	float orientation;
	char ammo; // TODO why is this a char?
} Player;

Player *Player_new(const char *name, SDL_Color color);

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

void Player_update(Player *player, float delta_time);

void Player_render(const Player *player, SDL_Renderer *renderer);

#endif // CASTROPARTY_PLAYER_H
