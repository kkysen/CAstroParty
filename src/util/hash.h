//
// Created by kkyse on 12/19/2017.
//

#ifndef SYSTEMS_HASH_H
#define SYSTEMS_HASH_H

#include <openssl/sha.h>
#include <stdio.h>
#include <stdint.h>

typedef unsigned char byte;

extern byte sha256_md[SHA256_DIGEST_LENGTH];

void sha256_r(const void *bytes, size_t size, void *digest);

void *sha256(const void *bytes, size_t size);

void fprint_sha256(FILE *file, const void *bytes);

int send_sha256(int fd, const void *bytes, size_t size);

int verify_sha256(int fd, const void *bytes, size_t size, const char *file_type);

typedef union {
    double d;
    uint64_t i;
} LongDouble;

uint64_t fp_hash(uint64_t hash, double val);

#define hash(h, val) fp_hash(h, (double) (val))

#define FNV1A_64_OFFSET_BASIS ((uint64_t) 14695981039346656037u)
#define FNV1A_64_PRIME ((uint64_t) 1099511628211u)

#define PRIME_64 FNV1A_64_PRIME

/**
 * Compute the 64-bit FNV 1-a hash of a string.
 *
 * @param [in] s the string to hash
 * @return the 64-bit FNV 1-a hash of the string
 */
uint64_t fnv1a_64_hash(const char *s);

#endif //SYSTEMS_HASH_H
