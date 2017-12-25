//
// Created by kkyse on 12/25/2017.
//

#ifndef CASTROPARTY_PLAYERS_H
#define CASTROPARTY_PLAYERS_H

#include <stdint.h>

#include "player.h"

typedef struct {
    uint8_t max_num_players;
    uint8_t num_players;
    Player *players;
} Players;

void Players_free(Players *players);

int Players_add(Players *players, Player *player);

void Players_update(Players *players, float delta_time);

void Players_render(const Players *players, SDL_Renderer *renderer);

#endif //CASTROPARTY_PLAYERS_H
