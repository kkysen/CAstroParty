#ifndef PLAYER_H
#define PLAYER_H

#include <SDL2/SDL.h>

#include <string.h>
#include <math.h>

#include "vector.h"

#define NAME_LENGTH 32
#define ACCELERATION 0.166667
#define ANGULAR_VELOCITY 6

struct player{
	char name[NAME_LENGTH];
	char* color;
	struct vector pos;
	struct vector velocity;
	float orientation;
	char ammo;
} typedef Player;

//creates new player
Player* new_player(char* name, char* color){
	Player init =
	(Player) {
		.color = color
	};
	Player* player = malloc(sizeof(Player));
	memcpy(player, &init, sizeof(Player));
	strncpy(player->name, name, NAME_LENGTH);
	return player;
}

//places player in world
void Player_spawn(Player* player, int* pos, float orientation, char ammo){

}

//-----------------------------------------------------------------------------

//accessor functions
char* Player_get_name(Player* player){
	return player->name;
}

char* Player_get_color(Player* player){
	return player->color;
}

struct vector Player_get_pos(Player* player){
	return player->pos;
}

struct vector Player_get_velocity(Player* player){
	return player->velocity;
}

float Player_get_orientation(Player* player){
	return player->orientation;
}

struct vector Player_get_acceleration(Player* player){
	return new_vector(ACCELERATION * cos(orientation*M_PI/180), ACCELERATION * sin(orientation*M_PI/180));
}

char Player_get_ammo(Player* player){
	return player->ammo;
}

//-----------------------------------------------------------------------------

//mutator functions
void Player_set_name(Player* player, char* name){
	strncpy(player->name, name, NAME_LENGTH);
}

void Player_set_color(Player* player, char* color){
	player->color = color;
}

void Player_set_pos(Player* player, struct vector pos){
	player->pos = pos;
}

void Player_set_velocity(Player* player, struct vector velocity){
	player->velocity = velocity;
}

void Player_set_orientation(Player* player, float orientation){
	player->orientation = orientation;
}

void Player_set_ammo(Player* player, char ammo){
	player->ammo = ammo;
}

//-----------------------------------------------------------------------------

//actions

void Player_update(Player* player){
	struct vector acceleration = get_acceleration(player);
	struct vector velocity = get_velocity(player);
	struct vector new_velocity = scale(vector_add(velocity, acceleration), 2.0/3.0);
	set_pos(player, new_pos);
	struct vector new_pos = vector_add(get_pos(player), get_velocity(player));
	set_pos(player, new_pos);
}

#endif
