//
// Created by kkyse on 12/20/2017.
//

#ifndef CASTRO_PARTY_SIZED_STRING_H
#define CASTRO_PARTY_SIZED_STRING_H

#include <stdlib.h>
#include <stdint.h>

typedef struct string {
    const size_t length;
    char *const chars;
    uint64_t hash;
} String;

extern const String BAD_STRING;

uint64_t String_hash(String s);

#endif // CASTRO_PARTY_SIZED_STRING_H
