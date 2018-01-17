//
// Created by kkyse on 12/20/2017.
//

#ifndef SYSTEMS_SIZED_STRING_H
#define SYSTEMS_SIZED_STRING_H

#include <stdlib.h>

typedef struct string {
    const size_t length;
    char *const chars;
} String;

extern const String BAD_STRING;

#endif //SYSTEMS_SIZED_STRING_H
