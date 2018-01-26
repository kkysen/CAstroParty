
#ifndef D_SERVER_HANDLER
#define D_SERVER_HANDLER

#define SERVER_PORT "8000"

void Server_init(int num_clients);
void Server_accept_connections();
void Server_tick();
void Server_quit();

#endif
