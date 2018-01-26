//
// Created by kkyse on 1/24/2018.
//

#include "player_serialization.h"

#include "serialization.h"
#include "../player.h"

size_t Player_serialized_length(Player *const player) {
    return 0
           + sizeof(player->id)
           + sizeof(player->position)
           + sizeof(player->velocity)
           + sizeof(player->orientation)
           + sizeof(player->ammo)
           + sizeof(player->sprite.id)
           + player->name.length
           + 0;
}

void Player_serialize_into(Player *const player, Buffer *const buffer) {
    Buffer_put(buffer, player->id);
    Buffer_put(buffer, player->position);
    Buffer_put(buffer, player->velocity);
    Buffer_put(buffer, player->orientation);
    Buffer_put(buffer, player->ammo);
    Buffer_put(buffer, player->sprite.id);
    Buffer_put_array(buffer, player->name.chars, player->name.length);
}

define_serializer(Player)

void Player_deserialize(Player *const player, Buffer *const buffer) {
    Buffer_get(buffer, player->id);
    Buffer_get(buffer, player->position);
    Buffer_get(buffer, player->velocity);
    Buffer_get(buffer, player->orientation);
    Buffer_get(buffer, player->ammo);
    Buffer_get(buffer, player->sprite.id);
    
    Buffer_get_array(buffer, player->name.chars, player->name.length);
    
    // these fields shouldn't be serialized and should be set to these default values
    set_field(player->is_own, false);
    set_field(player->socket_fd, -1);
}