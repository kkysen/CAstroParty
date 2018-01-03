//
// Created by kkyse on 12/20/2017.
//

#include "sized_string.h"
#include "hash.h"

const String BAD_STRING = {.length = 0, .chars = NULL};

uint64_t String_hash(String s) {
    if (s.hash == 0) {
        // this will recompute the hash every time if the FNV-1a hash == 0,
        // but that's not too bad considering its rarity
        s.hash = fnv1a_64_hash(s.chars);
    }
    return s.hash;
}