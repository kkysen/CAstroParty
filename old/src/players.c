//
// Created by kkyse on 12/25/2017.
//

#include "players.h"

#include "util/utils.h"
#include "util/hash.h"
#include "player.h"
#include "textures.h"
#include "serialize/buffer.h"

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
//    Vector_i_add(player->position, player->sprite.center); // TODO is this right
    player->velocity = Vector_new(0, 0);
    // TODO verify player initial values
    
    Player *const local_players = realloc(players->players, num_players * sizeof(Player));
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

int Players_load_sprites(Players *const players, SDL_Renderer *const renderer) {
    const uint_fast8_t num_players = players->num_players;
    const Player *const players_array = players->players;
    for (uint_fast8_t i = 0; i < num_players; ++i) {
        const Player *const player = players_array + i;
        const Sprite *const sprite = get_sprite(player->sprite.id, renderer);
        if (!sprite) {
            perror("invalid GameTexture or SDL_Renderer: get_sprite(player->sprite.id, renderer)");
            return -1;
        }
        set_field_memory(player->sprite, sprite);
    }
    return 0;
}

#define define_Players_have_field(field, Type, equals_expr) \
bool Players_have_##field(const Players *const players, const Type field) { \
    const uint_fast8_t num_players = players->num_players; \
    const Player *const players_array = players->players; \
    for (uint_fast8_t i = 0; i < num_players; ++i) { \
        const Player *const player = players_array + i; \
        if (equals_expr) { \
            return true; \
        } \
    } \
    return false; \
}

define_Players_have_field(name, String, String_equals(name, player->name));

define_Players_have_field(texture, GameTexture, texture == player->sprite.id);

#undef define_Players_have_field