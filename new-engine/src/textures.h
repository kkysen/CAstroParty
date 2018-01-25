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
    BULLET,
    NUM_PLAYERS,
} GameTexture;

typedef struct {
    const GameTexture id;
    SDL_Texture *const texture;
    const int width;
    const int height;
    const Vector center;
} Sprite;

const char *get_texture_name(GameTexture texture);

SDL_Texture *get_texture(GameTexture texture, SDL_Renderer *renderer);

void remove_texture(GameTexture texture);

Sprite *get_sprite(GameTexture texture, SDL_Renderer *renderer);

void remove_sprite(const Sprite *sprite);

int destroy_all_textures_on_exit();

#endif // CASTROPARTY_TEXTURES_H
