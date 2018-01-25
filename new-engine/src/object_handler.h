
#ifndef D_OBJECT_HANDLER
#define D_OBJECT_HANDLER

#include "player.h"
#include "bullet.h"

// Main handler functions
void ObjectHandler_init();

void ObjectHandler_tick();

void ObjectHandler_render();

// Game Object creation functions
Player *ObjectHandler_new_player(Vector position, int server_id);

Bullet *ObjectHandler_new_bullet(Vector position, float angle);

#endif
