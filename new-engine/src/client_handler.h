#include<stdbool.h>

#ifndef D_CLIENT_HANDLER
#define D_CLIENT_HANDLER

void Client_init();
void Client_tick();
void Client_quit();


// Client player index, provided by the server.
//  This is the "id" that the client and server use
//  to control the proper player
int Client_player_index;

bool Client_got_new_inputs; // Did we get new inputs?

struct Networking_unpacked_inputs *Client_unpacked_inputs; // Inputs from the server, unpacked

#endif
