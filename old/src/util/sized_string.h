//
// Created by kkyse on 12/20/2017.
//

#ifndef SYSTEMS_SIZED_STRING_H
#define SYSTEMS_SIZED_STRING_H

#include <stdlib.h>
#include <stdbool.h>

typedef struct string {
    const size_t length;
    char *const chars;
} String;

extern const String INVALID_STRING;

String String_new(const char *s);

int String_compare(String s1, String s2);

bool String_equals(String s1, String s2);

#endif //SYSTEMS_SIZED_STRING_H
