//
// Created by kkyse on 12/1/2017.
//

#ifndef SHELL_STRING_BUILDER_H
#define SHELL_STRING_BUILDER_H

#include <stdlib.h>
#include <stdio.h>

typedef struct string_builder {
    
    union {
        char *chars;
        
        void *ptr;
    };
    
    size_t size;
    
    size_t capacity;
    
} StringBuilder;

/**
 * Create a new StringBuilder with an \param initial_size.
 *
 * @param [in] initial_size the initial size and capacity of the StringBuilder
 * @return the StringBuilder
 */
StringBuilder *StringBuilder_new(size_t initial_size);

/**
 * Ensure this StringBuilder has a capacity of at least \param capacity.
 *
 * @param [in, out] sb this StringBuilder
 * @param [in] capacity the minimum capacity
 */
void StringBuilder_ensure_capacity(StringBuilder *sb, size_t capacity);

/**
 * Ensure this StringBuilder has a capacity of at least
 * the current size + \param more_capacity.
 *
 * @param [in, out] sb this StringBuilder
 * @param [in] more_capacity the minimum capacity to add to the current size
 */
void StringBuilder_ensure_more_capacity(StringBuilder *sb, size_t more_capacity);

/**
 * Shrink the capacity of this StringBuilder to the current size.
 *
 * @param [in, out] sb this StringBuilder
 */
void StringBuilder_shrink_to_size(StringBuilder *sb);

/**
 * Append \param size \param bytes to this StringBuilder.
 *
 * @param [in, out] sb this StringBuilder
 * @param [in] bytes the bytes to append
 * @param [in] size the size of the \param bytes
 */
void StringBuilder_append_bytes(StringBuilder *sb, void *bytes, size_t size);

/**
 * Append a string \param s of length \param size to this StringBuilder.
 *
 * @param [in, out] sb this StringBuilder
 * @param [in] s the string to append
 * @param [in] size the size of string \param s
 */
void StringBuilder_append_string_n(StringBuilder *sb, char *s, size_t size);

/**
 * Append a null-terminated string \param s to this StringBuilder.
 *
 * @param [in, out] sb this StringBuilder
 * @param [in] s the null-terminated string to append
 */
void StringBuilder_append_string(StringBuilder *sb, char *s);

/**
 * Append the contents of a \param file stream to this StringBuilder.
 *
 * @param [in, out] sb this StringBuilder
 * @param [in] file the file to append
 * @return the number of bytes read from the stream
 */
size_t StringBuilder_append_stream(StringBuilder *sb, FILE *file);

/**
 * Clear this StringBuilder,
 * freeing its chars and setting its size and capacity to 0.
 *
 * @param [in, out] sb this StringBuilder
 */
void StringBuilder_clear(StringBuilder *sb);

/**
 * Free this StringBuilder, clearing this StringBuilder and freeing its pointer.
 * @param [in, out] sb
 */
void StringBuilder_free(StringBuilder *sb);

#endif //SHELL_STRING_BUILDER_H
