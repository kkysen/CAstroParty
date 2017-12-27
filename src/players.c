//
// Created by kkyse on 12/25/2017.
//

#include "players.h"

#include "util/utils.h"
#include "util/hash.h"
#include "player.h"

void Players_free(Players *const players) {
    free(players->players);
    players->players = NULL;
    players->num_players = 0;
}

int Players_add(Players *const players, Player *const player) {
    const uint8_t num_players = players->num_players + (uint8_t) 1;
    if (num_players == 0) {
        // overflow
        perror("Player overflow");
        return -1;
    }
    if (num_players == players->max_num_players) {
        perror("Too many players");
        return -1;
    }
    
    player->id = num_players - (uint8_t) 1;
    player->velocity = Vector_new(0, 0);
    // TODO verify player initial values
    
    Player *const local_players = realloc(players->players, num_players);
    check_null_msg(local_players, "realloc(game->players, num_players)");
    memcpy(local_players + num_players - 1, player, sizeof(Player));
    players->players = local_players;
    players->num_players = num_players;
    
    return 0;
}

void Players_update(Players *const players, GameState *const state, const float delta_time) {
    const uint_fast8_t num_players = players->num_players;
    Player *const players_array = players->players;
    for (uint_fast8_t i = 0; i < num_players; ++i) {
        Player_update(players_array + i, state, delta_time);
    }
}

void Players_render(const Players *const players, const GameState *const state, SDL_Renderer *const renderer) {
    const uint_fast8_t num_players = players->num_players;
    const Player *const players_array = players->players;
    for (uint_fast8_t i = 0; i < num_players; ++i) {
        Player_render(players_array + i, state, renderer);
    }
}

uint64_t Players_hash(const Players *const players) {
    uint64_t hash = PRIME_64;
    const uint_fast8_t num_players = players->num_players;
    const Player *const players_array = players->players;
    for (uint_fast8_t i = 0; i < num_players; ++i) {
        hash = hash(hash, Player_hash(players_array + i));
    }
    return hash;
}