//
// Created by kkyse on 1/23/2018.
//

#include "networking.h"

#include <netdb.h>
#include <stdio.h>

#include "util/utils.h"

typedef int (*ConnectOrBind)(int socket_fd, const struct sockaddr *address, int address_length);

static int open_socket(IpPort ip_port, const ConnectOrBind connect_or_bind) {
    if (!(connect_or_bind == connect || connect_or_bind == bind)) {
        perror("connect_or_bind must be connect() or bind()");
        return -1;
    }
    
    struct addrinfo hints = {
            .ai_family = AF_UNSPEC, // IPv4 or IPv6
            .ai_socktype = SOCK_DGRAM, // UDP
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

int f() {
    struct addrinfo hints = {
            .ai_family = AF_UNSPEC, // IPv4 or IPv6
            .ai_socktype = SOCK_DGRAM, // UDP
            .ai_flags = AI_PASSIVE, // let syscall fill in
    };
    struct addrinfo *results;
    
    const char *const port = "3490";
    
    const int status = getaddrinfo(NULL, port, &hints, &results);
    if (status != 0) {
        fprintf(stderr, "%s: %s\n", gai_strerror(status), "getaddrinfo(NULL, port, &hints, &results)");
    }
    
    // really should walk the results linked list
    const int socket_fd = socket(results->ai_family, results->ai_socktype, results->ai_protocol);
    check_msg(socket_fd, "socket(results->ai_family, results->ai_socktype, results->ai_protocol)");
    
    check_perror(bind(socket_fd, results->ai_addr, results->ai_addrlen));
    
    const int yes = 1;
    check_perror(setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)));
    
    freeaddrinfo(results);
}