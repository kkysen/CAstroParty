#include<stdbool.h>

#ifndef D_CLIENT_HANDLER
#define D_CLIENT_HANDLER

void Client_init();
void Client_init_unpacked_data();
void Client_tick();
void Client_quit();


// Client player index, provided by the server.
//  This is the "id" that the client and server use
//  to control the proper player
int Client_player_index;

bool Client_got_new_inputs; // Did we get new inputs?
bool Client_got_new_data;   // Did we get new data, and do we need to update everything?

struct Networking_unpacked_inputs *Client_unpacked_inputs; // Inputs from the server, unpacked
struct Networking_unpacked_player *Client_unpacked_players; // Players from the server, unpacked


void Client_update_inputs(char *data);

#endif
