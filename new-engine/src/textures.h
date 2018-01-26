//
// Created by kkyse on 12/26/2017.
//

#ifndef CASTROPARTY_TEXTURES_H
#define CASTROPARTY_TEXTURES_H

#include <SDL2/SDL_render.h>

#include "vector.h"

typedef enum {
    INVALID_PLAYER = -1,
    BLUE_PLAYER = 0,
    GREEN_PLAYER,
    PURPLE_PLAYER,
    RED_PLAYER,
    NUM_PLAYERS,
    BULLET,
} GameTexture;

typedef struct {
    const GameTexture id;
    SDL_Texture *const texture;
    SDL_Color border_color;
    const int width;
    const int height;
    const Vector center;
    float angle;
} Sprite;

const char *get_texture_name(GameTexture texture);

SDL_Texture *get_texture(GameTexture texture, SDL_Renderer *renderer);

void remove_texture(GameTexture texture);

Sprite *get_sprite(GameTexture texture, SDL_Renderer *renderer);

void remove_sprite(const Sprite *sprite);

int destroy_all_textures_on_exit();

void Sprite_draw(const Sprite *sprite, Vector position, SDL_Renderer *renderer);

#endif // CASTROPARTY_TEXTURES_H
