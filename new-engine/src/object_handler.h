
#ifndef D_OBJECT_HANDLER
#define D_OBJECT_HANDLER

// Main handler functions
void ObjectHandler_init();
void ObjectHandler_tick();
void ObjectHandler_render();

// Game Object creation functions
struct player *ObjectHandler_new_player(double x, double y, int server_id);

#endif
