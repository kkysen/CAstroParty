//
// Created by kkyse on 12/19/2017.
//

#include "hash.h"

#include <stdint.h>
#include <string.h>

#include "io.h"

byte sha256_md[SHA256_DIGEST_LENGTH] = {0};

void sha256_r(const void *const bytes, const size_t size, void *const digest) {
    SHA256(bytes, size, digest);
}

void *sha256(const void *const bytes, const size_t size) {
    sha256_r(bytes, size, sha256_md);
    return sha256_md;
}

void fprint_sha256(FILE *const file, const void *const digest) {
    const byte *const digest_bytes = (byte *) digest;
    fprintf(file, "0x");
    for (uint_fast8_t i = 0; i < SHA256_DIGEST_LENGTH; ++i) {
        fprintf(file, "%x", digest_bytes[i]);
    }
}

int send_sha256(const int fd, const void *const bytes, const size_t size) {
    if (write_bytes(fd, sha256(bytes, size), SHA256_DIGEST_LENGTH) == -1) {
        return -1;
    }
    return 0;
}

int verify_sha256(const int fd, const void *const bytes, const size_t size, const char *const file_type) {
    byte received_hash[SHA256_DIGEST_LENGTH];
    if (read_bytes(fd, received_hash, SHA256_DIGEST_LENGTH) == -1) {
        fprintf(stderr, "Insecure %s: hashsum not received.\n", file_type);
        return -1;
    }
    
    const byte *own_hash = (byte *) sha256(bytes, size);
    
    if (memcmp(own_hash, received_hash, SHA256_DIGEST_LENGTH) != 0) {
        fprintf(stderr, "Insecure %s: hashsums are different: ", file_type);
        fprint_sha256(stderr, received_hash);
        fprintf(stderr, " != ");
        fprint_sha256(stderr, own_hash);
        fprintf(stderr, "\n");
        return -1;
    }
    
    printf("%s Verified: hashsum = ", file_type);
    fprint_sha256(stdout, own_hash);
    printf("\n");
    
    return 0;
}