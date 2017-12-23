#ifndef PLAYER_H
#define PLAYER_H

#include <SDL2/SDL.h>

#include <string.h>
#include <math.h>

#include "vector.h"

#define NAME_LENGTH 32
#define ACCELERATION 0.166667
#define MAX_SPEED 0.333333

struct player{
	char name[NAME_LENGTH];
	SDL_Color color;
	struct vector pos;
	struct vector velocity;
	float orientation;
	char ammo;
};

//creates new player
struct player* new_player(char* name, SDL_Color color){
	struct player init =
	(struct player) {
		.color = color
	};
	struct player* player = malloc(sizeof(struct player));
	memcpy(player, &init, sizeof(struct player));
	strncpy(player->name, name, NAME_LENGTH);
	return player;
}

//places player in world
void spawn(struct player* player, int* pos, float orientation, char ammo){

}

//-----------------------------------------------------------------------------

//accessor functions
char* get_name(struct player* player){
	return player->name;
}

SDL_Color get_color(struct player* player){
	return player->color;
}

struct vector get_pos(struct player* player){
	return player->pos;
}

struct vector get_velocity(struct player* player){
	return player->velocity;
}

float get_orientation(struct player* player){
	return player->orientation;
}

struct vector get_acceleration(struct player* player){
	return new_vector(ACCELERATION * cos(orientation*M_PI/180), ACCELERATION * sin(orientation*M_PI/180));
}

char get_ammo(struct player* player){
	return player->ammo;
}

//-----------------------------------------------------------------------------

//mutator functions
void set_name(struct player* player, char* name){
	strncpy(player->name, name, NAME_LENGTH);
}

void set_color(struct player* player, SDL_Color color){
	player->color = color;
}

void set_pos(struct player* player, struct vector pos){
	player->pos = pos;
}

void set_velocity(struct player* player, struct vector velocity){
	player->velocity = velocity;
}

void set_orientation(struct player* player, float orientation){
	player->orientation = orientation;
}

void set_ammo(struct player* player, char ammo){
	player->ammo = ammo;
}

//-----------------------------------------------------------------------------

//actions

void update(struct player* player){
	struct vector acceleration = get_acceleration(player);
	struct vector velocity = get_velocity(player);
	float speed = magnitude(velocity);
	struct vector new_velocity = scale(vector_add(velocity, acceleration), magnitude);
	set_pos(player, new_pos);
	struct vector new_pos = vector_add(get_pos(player), get_velocity(player));
	set_pos(player, new_pos);
}

#endif
