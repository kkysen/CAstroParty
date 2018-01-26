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
#include<sys/socket.h>
#include<netdb.h>
#include<errno.h>

static int server_socket;
static int *client_sockets; // List of clients
static size_t num_clients = 0;

// Initializes our server
void Server_init(const size_t _num_clients) {
    num_clients = _num_clients;
    client_sockets = malloc(num_clients * sizeof(int));
    
    struct addrinfo hints = {
            .ai_family = AF_UNSPEC,
            .ai_socktype = SOCK_STREAM,
            .ai_flags = AI_PASSIVE,
    };
    struct addrinfo *results;
    getaddrinfo(NULL, SERVER_PORT, &hints, &results);
    
    // Init server
    printf("Creating server...\n");
    server_socket = socket(results->ai_family, results->ai_socktype, results->ai_protocol);
    
    if (server_socket < 0) {
        printf("socket creation err\n");
        exit(-1);
    }
    
    if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &(int) {1}, sizeof(int)) < 0) {
        printf("setsockopt failed\n");
        exit(-1);
    }
    
    // Bind our address to our port
    const int bind_no = bind(server_socket, results->ai_addr, results->ai_addrlen);
    if (bind_no < 0) {
        printf("Bind error errno %d\n", errno);
        exit(-1);
    }
    
    freeaddrinfo(results);
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
    
    size_t client_count = 0;
    
    while (client_count < num_clients) {
        listen(server_socket, 4);
        client_sockets[client_count] = accept(server_socket, NULL, NULL);
        if (client_sockets[client_count] < 0) {
            printf("client socket accept/creation err: errno %d\n", errno);
            exit(-1);
        }
        
        const int flags = fcntl(client_sockets[client_count], F_GETFL, 0);
        fcntl(client_sockets[client_count], F_SETFL, flags | O_NONBLOCK);
        
        client_count++;
        printf("Client %zu/%zu connected!\n", client_count, num_clients);
    }
    
    // Now, send a confirmation to all clients so they can start playing
    //      We send an INDEX to all clients to identify them with
    char confirmation[2];
    confirmation[1] = (char) num_clients;
    for (size_t i = 0; i < num_clients; i++) {
        confirmation[0] = (char) i;
        send(client_sockets[i], confirmation, sizeof(confirmation), 0);
    }
    printf("All clients have connected!\n");
}

/** Server_tick()
 *  Receive input data from clients and send game data back to the clients
 */
void Server_tick() {
    
    // Receive inputs
    char buffer[num_clients];
    for (size_t i = 0; i < num_clients; i++) {
        // Todo: Check for reading null
        const ssize_t read_size = read(client_sockets[i], buffer + i, sizeof(*buffer));
        if (read_size == 0) {
            buffer[i] = -1; // No-input!
        }
    }
    
    // Send the data to all clients
    for (size_t i = 0; i < num_clients; i++) {
        if (buffer[i] == -1) {
            continue;
        }
        if (send(client_sockets[i], buffer, sizeof(buffer), MSG_DONTWAIT) == -1) {
            printf("Server Send to Client errorno %d\n", errno);
            exit(-1);
        }
    }
}

/** Server_quit()
 *      Cleans up the server, freeing all sockets
 */
void Server_quit() {
    close(server_socket);
    for (size_t i = 0; i < num_clients; i++) {
        close(client_sockets[i]);
    }
}
