//
// Created by kkyse on 1/24/2018.
//

#ifndef CASTROPARTY_GAME_SERIALIZATION_H
#define CASTROPARTY_GAME_SERIALIZATION_H

#include "../game2.h"
#include "../networking.h"

size_t Game_serialized_length(Game *game);

void Game_serialize_into(Game *game, Buffer *buffer);

Buffer Game_serialize(Game *game);

int Game_deserialize(Game *game, Buffer buffer);

#endif // CASTROPARTY_GAME_SERIALIZATION_H
