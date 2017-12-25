//
// Created by kkyse on 12/19/2017.
//

#ifndef SYSTEMS_HASH_H
#define SYSTEMS_HASH_H

#include <openssl/sha.h>
#include <stdio.h>

typedef unsigned char byte;

extern byte sha256_md[SHA256_DIGEST_LENGTH];

void sha256_r(const void *bytes, size_t size, void *digest);

void *sha256(const void *bytes, size_t size);

void fprint_sha256(FILE *file, const void *bytes);

int send_sha256(int fd, const void *bytes, size_t size);

int verify_sha256(int fd, const void *bytes, size_t size, const char *file_type);

#endif //SYSTEMS_HASH_H
