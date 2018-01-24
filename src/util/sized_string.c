//
// Created by kkyse on 12/20/2017.
//

#include "sized_string.h"

#include <string.h>

const String INVALID_STRING = {.length = 0, .chars = NULL};

int String_compare(const String s1, const String s2) {
    if (s1.length != s2.length) {
        return s1.length < s2.length ? -1 : 1;
    }
    if (s1.chars == s2.chars) {
        return 0;
    }
    return strncmp(s1.chars, s2.chars, s1.length);
}

bool String_equals(const String s1, const String s2) {
    return String_compare(s1, s2) == 0;
}