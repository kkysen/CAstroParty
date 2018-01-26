//
// Created by kkyse on 1/24/2018.
//

#ifndef CASTROPARTY_GAME_STATE_SERIALIZATION_H
#define CASTROPARTY_GAME_STATE_SERIALIZATION_H

#include "buffer.h"
#include "../game2.h"

size_t GameState_serialized_length(GameState *state);

void GameState_serialize_into(GameState *state, Buffer *buffer);

Buffer GameState_serialize(GameState *state);

void GameState_deserialize(GameState *state, Buffer *buffer);

#endif // CASTROPARTY_GAME_STATE_SERIALIZATION_H
