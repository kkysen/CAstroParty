
#include "client_handler.h"
#include "object_handler.h"
#include "input_handler.h"
#include "server_handler.h"
#include "networking.h"

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netdb.h>
#include<errno.h>
#include<stdbool.h>

// Read and write to this to read from and write to server
int Client_server_socket;

// How many clients there are in total (counting us)
int Client_server_total_number_of_clients;

/** Client_init( server_ip )
 *      Starts our client and connects to our server
 */
void Client_init(char *server_ip) {
 
    // Init client
    printf("Creating client...\n");
    Client_server_socket  = socket(AF_INET, SOCK_STREAM, 0);

    /*if (setsockopt(Client_server_socket, SOL_SOCKET, SO_REUSEADDR, &(int) { 1 }, sizeof(int)) < 0) {
        printf("setsockopt failed\n");
        exit(-1);
    }*/

    if (Client_server_socket < 0) {
        printf("socket creation err\n");
        exit(-1);
    }

    struct addrinfo *hints, *results;

    hints = (struct addrinfo *) calloc(1, sizeof(struct addrinfo));
    hints->ai_family = AF_INET; // IpV4
    hints->ai_socktype = SOCK_STREAM; // To be changed to datagram later
    hints->ai_flags = AI_PASSIVE; // Aerver only

    getaddrinfo(server_ip, SERVER_PORT, hints, &results);

    // Connect to server
    printf("Connecting to server...\n");
    int connect_val = connect(Client_server_socket, results->ai_addr, results->ai_addrlen);
    if (connect_val < 0) {
        printf("connect errno %d\n", errno);
        exit(-1);
    }

    // Make our socket non-blocking
    int flags = fcntl(Client_server_socket, F_GETFL, 0);
    fcntl(Client_server_socket, F_SETFL, flags | O_NONBLOCK);

    // Wait for server confirmation
    char result[2];
    while(1) {
        // If we've actually received something
        if (read(Client_server_socket, result, 2) > 1) {
            Client_player_index =                   result[0];
            Client_server_total_number_of_clients = result[1];
            break;
        }
    }
    printf("Connected and verified with server! Server gave us the ID: %d\n", Client_player_index);

    // NOW we make our players!
    int i = 0;
    for(; i < Client_server_total_number_of_clients; i++) {
        ObjectHandler_new_player(50*i,100, i); // Arbitrarily place them here
    }
}

/** void Client_init_unpacked_data()
 *      Initializes our client data, so we can hold our unpacked buffer data.
 *
 *      This data is read by the ObjectHandler to update all of our players
 */
void Client_init_unpacked_data() {
    // Make our unpacked input and player list
    Client_unpacked_inputs =  malloc(Client_server_total_number_of_clients * sizeof(struct Networking_unpacked_inputs));
    Client_unpacked_players = malloc(Client_server_total_number_of_clients * sizeof(struct Networking_unpacked_player));
    printf("init unpacked data...\n");
}

/** Client_tick()
 *      Send data to the server and read data from the server,
 *      then take that data and push it to our players so we
 *      can simulate them locally
 */
void Client_tick() {

    char send_buffer; // Sent to server
    char result[256];  // Received from server. 256 is arbitrary.

    send_buffer = Networking_compress_inputs( InputHandler_key_accelerate, InputHandler_key_turn_left, InputHandler_key_shoot );

    // Default state: No new inputs
    Client_got_new_inputs = false;
    Client_got_new_data = false;

    // If we've read something from the server
    if (read(Client_server_socket, result, sizeof(result)) > 1) {
        
        if (result[0] == SERVER_NETWORKING_INPUT_ID) {
            Client_update_inputs(&result[1]);
        } else if (result[0] == SERVER_NETWORKING_DATA_ID) {
            Client_got_new_data = true;

            int byte_counter = 1; // Byte counter, iterating through all bytes in the data.
                                  // The 1st byte is already read (skip through it)
                                  //TODO: Use byte_counter for input too
            int num_players = result[byte_counter++];

            int player_counter = 0;
            for(;player_counter < num_players; player_counter++) {
                // Copy the memory of our result buffer into our player list
                memcpy( &Client_unpacked_players[player_counter], &result[ byte_counter ], sizeof(struct Networking_unpacked_player));
                byte_counter += sizeof(struct Networking_unpacked_player);
            }
        } else {
            printf("Server sent bad code: %d\n", result[0]);
        }
    }


    // Send our buffer to the server
    if (send(Client_server_socket, &send_buffer, sizeof(send_buffer), MSG_DONTWAIT) == -1) {
        printf("Send errorno %d\n", errno);
        exit(-1);
    }


    /*char buffer[1024];
    char buffer_response[1024];
    while(1) {
        printf("prompt: ");
        fgets(buffer, 1024, stdin);
        printf("size: %d\n", strlen(buffer));
        // Send data to server
        send(sd, buffer, sizeof(buffer), 0);
        if (send(sd, buffer, sizeof(buffer), MSG_DONTWAIT) == -1) {
            printf("Send errorno %d\n", errno);
            exit(-1);
        }
        // Get server response
        int size = read(sd, buffer_response, sizeof(buffer_response));
        buffer_response[size] = 0;
        printf("Got server response: %s\n", buffer_response);
    }*/
}

/** Client_update_inputs( data )
 *      Updates the client inputs from a bunch of ordered input data
 */
void Client_update_inputs(char *data) {

    Client_got_new_inputs = true;

    // Fill in our unpacked list of inputs
    int i = 0;
    for(;i < Client_server_total_number_of_clients; i++) {
        Client_unpacked_inputs[i] = *Networking_decompress_inputs(data[i]);
    }
}

void Client_quit() {
    printf("Closing Client\n");
    close(Client_server_socket);
    // TODO: free(hints);
    // TODO: freeaddrinfo(results);
}
