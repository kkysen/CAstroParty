//
// Created by kkyse on 1/24/2018.
//

#include "players_serialization.h"

#include "serialization.h"
#include "player_serialization.h"

size_t Players_serialized_length(Players *const players) {
    size_t length = sizeof(players->max_num_players) + sizeof(players->num_players);
    for (uint_fast8_t i = 0; i < players->num_players; i++) {
        length += Player_serialized_length(players->players + i);
    }
    return length;
}

void Players_serialize_into(Players *const players, Buffer *const buffer) {
    Buffer_put(buffer, players->max_num_players);
    Buffer_put(buffer, players->num_players);
    for (uint_fast8_t i = 0; i < players->num_players; i++) {
        Player_serialize_into(players->players + i, buffer);
    }
}

define_serializer(Players)

void Players_deserialize(Players *const players, Buffer *const buffer) {
    Buffer_get(buffer, players->max_num_players);
    Buffer_get(buffer, players->num_players);
    players->players = (Player *) realloc(players->players, players->num_players * sizeof(Player));
    for (uint_fast8_t i = 0; i < players->num_players; i++) {
        Player_deserialize(players->players + i, buffer);
    }
}