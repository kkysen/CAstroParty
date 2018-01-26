/** server_handler.c
 *
 *  Handles the server side:
 *      - Initializing the server
 *      - Handling client connections
 *      - Sending data to the clients
 *      - Receiving data from the clients and updating the server players
 */

#include "server_handler.h"

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netdb.h>
#include<errno.h>


int Server_server_socket;
int *Server_client_sockets; // List of clients
int Server_number_of_clients;


// Initializes our server
void Server_init(int num_clients) {
    Server_number_of_clients = num_clients;
    Server_client_sockets = malloc(num_clients * sizeof(int));

    // Init server
    printf("Creating server...\n");
    Server_server_socket = socket(AF_INET, SOCK_STREAM, 0);

    if (setsockopt(Server_server_socket, SOL_SOCKET, SO_REUSEADDR, &(int) { 1 }, sizeof(int)) < 0) {
        printf("setsockopt failed\n");
        exit(-1);
    }

    if (Server_server_socket < 0) {
        printf("socket creation err\n");
        exit(-1);
    }

    struct addrinfo *hints, *results;

    hints = (struct addrinfo *) calloc(1, sizeof(struct addrinfo));
    hints->ai_family = AF_INET; // IpV4
    hints->ai_socktype = SOCK_STREAM; // To be changed to datagram later
    hints->ai_flags = AI_PASSIVE; // Aerver only

    getaddrinfo(NULL, SERVER_PORT, hints, &results);

    // Bind our address to our port
    int bindno = bind(Server_server_socket, results->ai_addr, results->ai_addrlen);
    if (bindno < 0) {
        printf("Bind error errno %d\n", errno);
        exit(-1);
    }

}

/** Server_accept_connections()
 *
 * Accept our client connections, until
 * our list of clients is full.
 *
 * These clients will be NON-BLOCKING,
 * which makes it possible to loop through em
 */
void Server_accept_connections() {
    printf("SERVER: Waiting for connections...\n");

    int client_count = 0;

    while(client_count < Server_number_of_clients) {
        socklen_t client_address_size;
        struct sockaddr_storage client_address;

        listen(Server_server_socket, 4);

        Server_client_sockets[client_count] = accept(
                Server_server_socket, 
                (struct sockaddr *) &client_address,
                &client_address_size
        );

        if (Server_client_sockets[client_count] < 0) {
            printf("client socket accept/creation err: errno %d\n", errno);
            exit(-1);
        }

        int flags = fcntl(Server_client_sockets[client_count], F_GETFL, 0);
        fcntl(Server_client_sockets[client_count], F_SETFL, flags | O_NONBLOCK);

        client_count++;
        printf("Client %d/%d connected!\n", client_count, Server_number_of_clients);
    }

    // Now, send a confirmation to all clients so they can start playing
    //      We send an INDEX to all clients to identify them with
    char confirmation[2];
    confirmation[1] = Server_number_of_clients;
    int i = 0;
    for(; i < Server_number_of_clients; i++) {
        confirmation[0] = i;
        send(Server_client_sockets[i], confirmation, sizeof(confirmation), 0);
    }
    printf("All clients have connected!\n");
}

/** Server_tick()
 *  Receive input data from clients and send game data back to the clients
 */
void Server_tick() { 
    
    // Receive inputs
    char buffer[Server_number_of_clients];
    int i = 0;
    for(; i < Server_number_of_clients; i++) {
        // Todo: Check for reading null
        int read_size = read( Server_client_sockets[i], &buffer[i], sizeof(buffer[i]) );
        if (read_size == 0) {
            buffer[i] = -1; // No-input!
        }
    }

    // Send the data to all clients
    i = 0;
    for(; i < Server_number_of_clients; i++) {
        if (buffer[i] == -1) continue;

        if (send(Server_client_sockets[i], buffer, sizeof(buffer), MSG_DONTWAIT) == -1) {
            printf("Server Send to Client errorno %d\n", errno);
            exit(-1);
        }
    }
}

/** Server_quit()
 *      Cleans up the server, freeing all sockets
 */
void Server_quit() {
    close(Server_server_socket);
    int i = 0;
    for(;i < Server_number_of_clients; i++) {
        close(Server_client_sockets[i]);
    }
}
