
#ifndef D_OBJECT_HANDLER
#define D_OBJECT_HANDLER

#define OBJECT_HANDLER_PLAYER_BUFFER_SIZE 10

// Player Game Object list. Initialize all to NULL
struct player *ObjectHandler_players[OBJECT_HANDLER_PLAYER_BUFFER_SIZE];

// Main handler functions
void ObjectHandler_init();
void ObjectHandler_tick();
void ObjectHandler_render();

// Game Object creation functions
struct player *ObjectHandler_new_player(double x, double y, int server_id);

#endif
