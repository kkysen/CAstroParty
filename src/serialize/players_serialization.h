//
// Created by kkyse on 1/24/2018.
//

#ifndef CASTROPARTY_PLAYERS_SERIALIZATION_H
#define CASTROPARTY_PLAYERS_SERIALIZATION_H

#include "buffer.h"
#include "../players.h"

size_t Players_serialized_length(Players *players);

void Players_serialize_into(Players *players, Buffer *buffer);

Buffer Players_serialize(Players *players);

void Players_deserialize(Players *players, Buffer *buffer);

#endif // CASTROPARTY_PLAYERS_SERIALIZATION_H
