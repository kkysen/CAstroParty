//
// Created by kkyse on 1/24/2018.
//

#ifndef CASTROPARTY_PLAYER_SERIALIZATION_H
#define CASTROPARTY_PLAYER_SERIALIZATION_H

#include "buffer.h"
#include "../player.h"

size_t Player_serialized_length(Player *player);

void Player_serialize_into(Player *player, Buffer *buffer);

Buffer Player_serialize(Player *player);

void Player_deserialize(Player *player, Buffer *buffer);

#endif // CASTROPARTY_PLAYER_SERIALIZATION_H
