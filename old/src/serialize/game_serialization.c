//
// Created by kkyse on 1/24/2018.
//

#include "game_serialization.h"

#include "serialization.h"
#include "game_state_serialization.h"
#include "../game.h"

size_t Game_serialized_length(Game *const game) {
    return 0
           + sizeof(game->title.length)
           + GameState_serialized_length(&game->state)
           + 0;
}

void Game_serialize_into(Game *const game, Buffer *const buffer) {
    Buffer_put_array(buffer, game->title.chars, game->title.length);
    GameState_serialize_into(&game->state, buffer);
}

define_serializer(Game)

void Game_deserialize(Game *const game, Buffer *const buffer) {
    Buffer_get_array(buffer, game->title.chars, game->title.length);
    GameState_deserialize(&game->state, buffer);
    
    // defaults
    set_field(game->render, true);
    set_field(game->is_client, true);
    set_field(game->server, NULL);
    set_field(game->client, NULL);
    game->prev_time = 0;
    game->interrupt = NULL;
    game->is_running = true;
    game->quit = false;
}