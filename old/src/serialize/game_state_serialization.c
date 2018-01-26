//
// Created by kkyse on 1/24/2018.
//

#include "game_state_serialization.h"

#include "serialization.h"
#include "players_serialization.h"

size_t GameState_serialized_length(GameState *const state) {
    return 0
           + sizeof(state->fps)
           + sizeof(state->tick)
           + sizeof(state->width)
           + sizeof(state->height)
           + Players_serialized_length(state->players)
           + 0;
}

void GameState_serialize_into(GameState *const state, Buffer *const buffer) {
    Buffer_put(buffer, state->fps);
    Buffer_put(buffer, state->tick);
    
    Buffer_put(buffer, state->width);
    Buffer_put(buffer, state->height);
    
    Buffer_put(buffer, state->own_player->id);
    Players_serialize_into(state->players, buffer);
}

define_serializer(GameState)

void GameState_deserialize(GameState *const state, Buffer *const buffer) {
    Buffer_get(buffer, state->fps);
    Buffer_get(buffer, state->tick);
    
    Buffer_get(buffer, state->width);
    Buffer_get(buffer, state->height);
    state->_x = 0; // default
    state->_y = 0; // default
    // calculate size in floats from integer width and height
    const Vector size = Vector_new(state->width, state->height);
    set_field(state->size, size);
    
    Players_deserialize(state->players, buffer);
    
    set_field(state->own_player, NULL); // default
}
