//
// Created by kkyse on 1/23/2018.
//

#include <netdb.h>
#include "networking.h"

int f() {
    struct addrinfo hints = {
            .ai_family = AF_UNSPEC, // IPv4 or IPv6
            .ai_socktype = SOCK_DGRAM, // UDP
            .ai_flags = AI_PASSIVE, // let syscall fill in
    };
    struct addrinfo *servinfo;
    
    const char *const port = "3490";
    
    const int status = getaddrinfo(NULL, port, &hints, &servinfo);
}