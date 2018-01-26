
#include "client_handler.h"

#include <errno.h>

#include "object_handler.h"
#include "input_handler.h"
#include "server_handler.h"
#include "util/utils.h"
#include "game.h"

#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netdb.h>

// Read and write to this to read from and write to server
static int server_socket;

// How many clients there are in total (counting us)
size_t num_clients;

/** Client_init( server_ip )
 *      Starts our client and connects to our server
 */
void Client_init(const char *const server_ip) {
    
    // Init client
    printf("Creating client...\n");
    
    struct addrinfo hints = {
            .ai_family = AF_UNSPEC,
            .ai_socktype = SOCK_STREAM,
            .ai_flags = AI_PASSIVE,
    };
    
    struct addrinfo *results;
    getaddrinfo(server_ip, SERVER_PORT, &hints, &results);
    
    server_socket = socket(results->ai_family, results->ai_socktype, results->ai_protocol);
    
    /*if (setsockopt(Client_server_socket, SOL_SOCKET, SO_REUSEADDR, &(int) { 1 }, sizeof(int)) < 0) {
        printf("setsockopt failed\n");
        exit(-1);
    }*/
    
    if (server_socket < 0) {
        printf("socket creation err\n");
        exit(-1);
    }
    
    // Connect to server
    printf("Connecting to server...\n");
    int connect_val = connect(server_socket, results->ai_addr, results->ai_addrlen);
    if (connect_val < 0) {
        printf("connect errno %d\n", errno);
        exit(-1);
    }
    
    freeaddrinfo(results);
    
    // Make our socket non-blocking
    int flags = fcntl(server_socket, F_GETFL, 0);
    fcntl(server_socket, F_SETFL, flags | O_NONBLOCK);
    
    // Wait for server confirmation
    char result[2];
    while (1) {
        // If we've actually received something
        if (read(server_socket, result, 2) > 1) {
            Client_player_index = (size_t) result[0];
            num_clients = (size_t) result[1];
            break;
        }
    }
    printf("Connected and verified with server! Server gave us the ID: %zd\n", Client_player_index);
    
    // Make our unpacked input list
    Client_unpacked_inputs = malloc(num_clients * sizeof(struct Networking_unpacked_inputs));
    
    ObjectHandler_init(num_clients);
}

/** Client_tick()
 *      Send data to the server and read data from the server,
 *      then take that data and push it to our players so we
 *      can simulate them locally
 */
void Client_tick() {
    
    char send_buffer; // Sent to server
    char result[num_clients * 2]; // * 2 to be safe
    
    send_buffer = Networking_compress_inputs(InputHandler_button_turn, InputHandler_button_shoot, InputHandler_button_restart);
    
    // If we've read something from the server
    if (read(server_socket, result, sizeof(result)) > 1) {
        Client_got_new_inputs = true;
        
        // Fill in our unpacked list of inputs
        for (size_t i = 0; i < num_clients; i++) {
            Client_unpacked_inputs[i] = *Networking_decompress_inputs(result[i]);
        }
    } else {
        Client_got_new_inputs = false;
    }
    
    // Send our buffer to the server
    if (send(server_socket, &send_buffer, sizeof(send_buffer), MSG_DONTWAIT) == -1) {
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

void Client_quit() {
    printf("Closing Client\n");
    close(server_socket);
}
