//
// Created by kkyse on 11/26/2017.
//

#include "string_utils.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "utils.h"

char *str_copy_n(const char *const s, const size_t length) {
    const size_t size = (length + 1) * sizeof(char);
    char *const copy = (char *) malloc(size);
    memcpy(copy, s, size - sizeof(char));
    copy[length] = 0;
    return copy;
}

char *str_copy(const char *const s) {
    return str_copy_n(s, strlen(s));
}

__attribute_pure__ bool str_contains(const char *s, const char *substring) {
    return strstr(s, substring) != NULL;
}

__attribute_pure__ size_t str_num_occurrences(const char *s, const char *const substring) {
    size_t n = 0;
    while ((s = strstr(s, substring))) {
        ++s;
        ++n;
    }
    return n;
}

char *str_replace(const char *const s, const char *const to_replace, const char *const replacement,
                  const size_t start, const size_t num_times) {
    p("str_replace");
    pz(str_num_occurrences(s, to_replace));
    const size_t num_replacements = min(num_times, str_num_occurrences(s, to_replace));
    if (num_replacements == 0) {
        return str_copy(s);
    }
    
    const size_t original_length = strlen(s);
    const size_t to_replace_length = strlen(to_replace);
    const size_t replacement_length = strlen(replacement);
    const ssize_t additional_length = replacement_length - to_replace_length;
    const size_t new_length = original_length + (additional_length * num_replacements);
    
    char *const new_str_start = (char *) malloc((new_length + 1) * sizeof(char));
    char *new_str = new_str_start;
    memcpy_add(new_str, s, start);
    
    const char *old_str = s + start;
    for (size_t i = 0; i < num_replacements; ++i) {
        const char *const found = strstr(old_str, to_replace);
        if (!found) {
            break;
        }
        memcpy_add(new_str, old_str, found - old_str);
        memcpy_add(new_str, replacement, replacement_length);
        old_str = found + 1;
    }
    
    memcpy(new_str, old_str, original_length - (old_str - s));
    
    new_str_start[new_length] = 0;
    ps(new_str_start);
    return new_str_start;
}

char *str_join_with_ends(const char *const delimiter, const char *const *const strings, const size_t num_strings,
               const char *const start, const char *const end) {
    const size_t start_length = strlen(start);
    const size_t end_length = strlen(end);
    
    const size_t delimiter_length = strlen(delimiter);
    size_t total_length = start_length + end_length + ((num_strings - 1) * delimiter_length);
    
    size_t *const lengths = (size_t *) malloc(num_strings * sizeof(size_t));
    for (size_t i = 0; i < num_strings; ++i) {
        const size_t length = strlen(strings[i]);
        total_length += length;
        lengths[i] = length;
    }
    
//    printf("total_length: %zu\n", total_length);
    char *const new_str_start = (char *) malloc((total_length + 1) * sizeof(char));
    
    char *new_str = new_str_start;
    memcpy_add(new_str, start, start_length);
    
    for (size_t i = 0;; ++i) {
        memcpy_add(new_str, strings[i], lengths[i]);
        if (i == num_strings - 1) {
            break;
        }
        memcpy_add(new_str, delimiter, delimiter_length);
    }
    free(lengths);
    
    memcpy_add(new_str, end, end_length);
    
    new_str_start[total_length] = 0;
    return new_str_start;
}

char *str_join(const char *const delimiter, const char *const *const strings, const size_t num_strings) {
    return str_join_with_ends(delimiter, strings, num_strings, "", "");
}

#define end_char_is(s, length, char) ((s)[0] == char && (s)[(length) - 1] == char)

char *unquote_n(char *const s, const size_t length) {
    if (end_char_is(s, length, '\'') || end_char_is(s, length, '"')) {
        s[length - 1] = 0; // cut off end
        return s + 1; // cut off front
    }
    return s;
}

char *unquote(char *const s) {
    return unquote_n(s, strlen(s));
}

char *strip_n(char *const s, const size_t length) {
    char c;
    
    char *end = s + length;
    while ((c = *--end) && is_whitespace(c));
    end[1] = 0;
    
    char *start = s - 1;
    while ((c = *++start) && is_whitespace(c));
    
    return start;
}

char *strip(char *const s) {
    return strip_n(s, strlen(s));
}