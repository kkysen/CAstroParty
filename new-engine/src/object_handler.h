
#ifndef D_OBJECT_HANDLER
#define D_OBJECT_HANDLER

#include "player.h"
#include "bullet.h"

// Main handler functions
void ObjectHandler_init();

void ObjectHandler_tick();

void ObjectHandler_render();

// Game Object creation functions
Player *ObjectHandler_new_player(float x, float y, int server_id);

Bullet *ObjectHandler_new_bullet(Vector position);

#endif
