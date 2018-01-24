//
// Created by kkyse on 1/23/2018.
//

#ifndef CASTROPARTY_NETWORKING_H
#define CASTROPARTY_NETWORKING_H

#include <unistd.h>

#ifndef MSG_MORE
    #define MSG_MORE 0x8000
#endif

typedef struct {
    const char *const ip_address;
    const char *const port;
} IpPort;

typedef struct {
    void *data;
    size_t index;
    ssize_t length;
} Buffer;

/**
 * Opens and binds to a socket to a local \param port.
 *
 * @param [in] port the local port to bind to
 * @return the socket file descriptor, or -1 if error
 */
int bind_to_socket(const char *port);

/**
 * Opens, binds, and listens to a socket to a local \param port.
 *
 * @param [in] port the local port to listen to
 * @return the socket file descriptor, or -1 if error
 */
int listen_to_socket(const char *port);

/**
 * Opens and connects to a socket to an \param ip_port.
 *
 * @param [in] ip_port the IP address and port to connect to
 * @return the socket file descriptor, or -1 if error
 */
int connect_to_socket(IpPort ip_port);

int send_all(int socket_fd, Buffer buffer);

int recv_all_into(int socket_fd, Buffer *buffer);

Buffer recv_all(int socket_fd);

int send_acknowledgement(int socket_fd);

int check_acknowledgement(int socket_fd);

#endif // CASTROPARTY_NETWORKING_H
