//
// Created by kkyse on 1/23/2018.
//

#include "networking.h"

#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <malloc.h>

#include "util/utils.h"

const Buffer INVALID_BUFFER = {
        .data = NULL,
        .index = 0,
        .length = -1,
};

typedef int (*ConnectOrBind)(int socket_fd, const struct sockaddr *address, int address_length);

static int open_socket(IpPort ip_port, const ConnectOrBind connect_or_bind) {
    if (!(connect_or_bind == connect || connect_or_bind == bind)) {
        perror("connect_or_bind must be connect() or bind()");
        return -1;
    }
    
    struct addrinfo hints = {
            .ai_family = AF_UNSPEC, // IPv4 or IPv6
            .ai_socktype = SOCK_STREAM, // TCP for now, might change later (TODO)
            .ai_flags = AI_PASSIVE, // let syscall fill in
    };
    struct addrinfo *result;
    
    const int status = getaddrinfo(ip_port.ip_address, ip_port.port, &hints, &result);
    if (status != 0) {
        fprintf(stderr, "%s: %s\n", gai_strerror(status), "getaddrinfo(NULL, port, &hints, &result)");
    }
    
    // really should walk the results linked list
    const int socket_fd = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (socket_fd == -1) {
        freeaddrinfo(result);
        check_msg(socket_fd, "socket(result->ai_family, result->ai_socktype, result->ai_protocol)");
    }
    
    if (connect_or_bind(socket_fd, result->ai_addr, result->ai_addrlen) == -1) {
        freeaddrinfo(result);
        check_msg(-1, "connect_or_bind(socket_fd, result->ai_addr, result->ai_addrlen)");
    }
    
    return socket_fd;
}

int bind_to_socket(const char *const port) {
    const IpPort ip_port = {
            .ip_address = NULL,
            .port = port,
    };
    
    const int socket_fd = open_socket(ip_port, bind);
    check_msg(socket_fd, "open_socket(ip_port, bind)");
    
    // allows socket to be reused
    const int yes = 1;
    check_perror(setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)));
    
    return socket_fd;
}

int connect_to_socket(IpPort ip_port) {
    const int socket_fd = open_socket(ip_port, connect);
    check_msg(socket_fd, "open_socket(ip_port, connect)");
    return socket_fd;
}

static const int LISTENING_BACKLOG = 10;

int listen_to_socket(const char *const port) {
    const int socket_fd = bind_to_socket(port);
    check_msg(socket_fd, "bind_to_socket(port)");
    
    check_perror(listen(socket_fd, LISTENING_BACKLOG));
    
    return socket_fd;
}

int send_all(const int socket_fd, const Buffer buffer) {
    if (socket_fd < 0) {
        perror("invalid socket");
        return -1;
    }
    
    ssize_t size = buffer.length - buffer.index;
    if (size < 0) {
        perror("negative buffer remaining");
        return -1;
    }
    if (size == 0) {
        return 0; // no data to send
    }
    
    if (send(socket_fd, &size, sizeof(size), MSG_MORE) != sizeof(size)) {
        perror("couldn't send size");
        return -1;
    }
    
    const void *const data = buffer.data + buffer.index;
    size_t i = 0;
    while (size > 0) {
        const ssize_t bytes_sent = send(socket_fd, data + i, (size_t) size, 0);
        if (bytes_sent < -1) {
            perror("send failure");
            return -1;
        }
        size -= bytes_sent;
        i += bytes_sent;
    }
    return 0;
}

static int recv_all_into_with_size(const int socket_fd, Buffer *const buffer, ssize_t size) {
    if (buffer->index + size >= buffer->length) {
        perror("buffer overflow");
        return -1;
    }
    
    void *const data = buffer->data + buffer->index;
    size_t i = 0;
    while (size > 0) {
        const ssize_t bytes_received = recv(socket_fd, data + i, (size_t) size, 0);
        if (bytes_received < 0) {
            perror("receive failure");
            return -1;
        }
        size -= bytes_received;
        i += bytes_received;
    }
    buffer->index += i;
    
    return 0;
}

int recv_all_into(const int socket_fd, Buffer *const buffer) {
    if (socket_fd < 0) {
        perror("invalid socket");
        return -1;
    }
    
    ssize_t size;
    if (recv(socket_fd, &size, sizeof(size), 0) != sizeof(size)) {
        perror("couldn't receive size");
        return -1;
    }
    
    if (size < 0) {
        perror("received negative size");
        return -1;
    }
    
    if (size == 0) {
        return 0;
    }
    
    if (recv_all_into_with_size(socket_fd, buffer, size) == -1) {
        perror("recv_all_into_with_size(socket_fd, buffer, size)");
        return -1;
    }
    return 0;
}

Buffer recv_all(const int socket_fd) {
    if (socket_fd < 0) {
        perror("invalid socket");
        return INVALID_BUFFER;
    }
    
    ssize_t size;
    if (recv(socket_fd, &size, sizeof(size), 0) != sizeof(size)) {
        perror("couldn't receive size");
        return INVALID_BUFFER;
    }
    
    if (size < 0) {
        perror("received negative size");
        return INVALID_BUFFER;
    }
    
    if (size == 0) {
        return (Buffer) {.data = NULL, .index = 0, .length = 0};
    }
    
    Buffer buffer = {
            .data = malloc((size_t) size),
            .index = 0,
            .length = size,
    };
    if (!buffer.data) {
        perror("malloc");
        return INVALID_BUFFER;
    }
    
    if (recv_all_into_with_size(socket_fd, &buffer, size) == -1) {
        perror("recv_all_into_with_size(socket_fd, &buffer, size)");
        free(buffer.data);
        return INVALID_BUFFER;
    }
    return buffer;
}