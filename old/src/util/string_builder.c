//
// Created by kkyse on 12/1/2017.
//

#include "string_builder.h"

#include <string.h>

#include "utils.h"

#define sb_terminate() sb->chars[sb->size] = 0

StringBuilder *StringBuilder_new(const size_t initial_size) {
    StringBuilder *const sb = (StringBuilder *) malloc(sizeof(StringBuilder));
    sb->size = initial_size;
    sb->capacity = initial_size;
    sb->ptr = malloc(initial_size + 1);
    sb_terminate();
    return sb;
}

void StringBuilder_ensure_capacity(StringBuilder *const sb, const size_t capacity) {
    if (sb->capacity < capacity) {
        sb->capacity = capacity;
        sb->ptr = realloc(sb->ptr, capacity + 1);
        sb_terminate();
    }
}

void StringBuilder_ensure_more_capacity(StringBuilder *const sb, const size_t more_capacity) {
    StringBuilder_ensure_capacity(sb, more_capacity + sb->size);
}

void StringBuilder_shrink_to_size(StringBuilder *const sb) {
    sb->capacity = sb->size;
    sb->ptr = realloc(sb->ptr, sb->capacity + 1);
    sb_terminate();
}

void StringBuilder_append_bytes(StringBuilder *const sb, void *const restrict bytes, const size_t size) {
    const size_t remaining = sb->capacity - sb->size;
    if (remaining < size) {
        sb->capacity = sb->size + size;
        sb->ptr = realloc(sb->ptr, sb->capacity + 1);
    }
    memcpy(sb->ptr + sb->size, bytes, size);
    sb->size += size;
    sb_terminate();
}

void StringBuilder_append_string_n(StringBuilder *const sb, char *const s, const size_t size) {
    StringBuilder_append_bytes(sb, s, size * sizeof(char));
}

void StringBuilder_append_string(StringBuilder *const sb, char *const s) {
    StringBuilder_append_string_n(sb, s, strlen(s));
}

size_t StringBuilder_append_stream(StringBuilder *const sb, FILE *const file) {
    char buffer[4096] = {0};
    size_t total_num_bytes = 0;
    size_t num_bytes;
    while ((num_bytes = fread(buffer, 1, sizeof(buffer) - 1, file)) > 0) {
        total_num_bytes += num_bytes;
        StringBuilder_append_bytes(sb, buffer, num_bytes);
    }
    return total_num_bytes;
}

void StringBuilder_clear(StringBuilder *const sb) {
    free(sb->ptr);
    sb->ptr = NULL;
    sb->size = sb->capacity = 0;
}

void StringBuilder_free(StringBuilder *const sb) {
    StringBuilder_clear(sb);
    free(sb);
}