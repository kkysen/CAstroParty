
#ifndef D_OBJECT_HANDLER
#define D_OBJECT_HANDLER

#include "player.h"
#include "bullet.h"

// Main handler functions
void ObjectHandler_init(size_t num_clients);

void ObjectHandler_tick();

void ObjectHandler_render();

void ObjectHandler_restart();

// Game Object creation functions
Player *ObjectHandler_new_player(Vector position, size_t server_id);

Bullet *ObjectHandler_new_bullet(Vector position, float angle, size_t player_server_index);

#endif
