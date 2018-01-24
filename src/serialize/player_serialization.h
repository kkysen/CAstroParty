//
// Created by kkyse on 1/24/2018.
//

#ifndef CASTROPARTY_PLAYER_SERIALIZATION_H
#define CASTROPARTY_PLAYER_SERIALIZATION_H

#include "../networking.h"
#include "../player.h"

size_t Player_serialized_length(Player *Player);

void Player_serialize_into(Player *Player, Buffer *buffer);

Buffer Player_serialize(Player *Player);

int Player_deserialize(Player *Player, Buffer buffer);

#endif // CASTROPARTY_PLAYER_SERIALIZATION_H
