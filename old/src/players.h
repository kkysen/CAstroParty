//
// Created by kkyse on 12/25/2017.
//

#ifndef CASTROPARTY_PLAYERS_H
#define CASTROPARTY_PLAYERS_H

#include <stdint.h>

typedef struct players Players;

#include "player.h"
#include "game.h"

struct players {
    uint8_t max_num_players;
    uint8_t num_players;
    Player *players;
};

void Players_free(Players *players);

int Players_add(Players *players, Player *player);

void Players_update(Players *players, GameState *state, float delta_time);

void Players_render(const Players *players, const GameState *state, SDL_Renderer *renderer);

uint64_t Players_hash(const Players *players);

int Players_load_sprites(Players *players, SDL_Renderer *renderer);

bool Players_have_name(const Players *players, String name);

bool Players_have_texture(const Players *players, GameTexture texture);

#endif //CASTROPARTY_PLAYERS_H
