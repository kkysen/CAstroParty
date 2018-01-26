//
// Created by kkyse on 1/24/2018.
//

#ifndef CASTROPARTY_BUFFER_H
#define CASTROPARTY_BUFFER_H

#include <unistd.h>

typedef struct {
    void *data;
    size_t index;
    ssize_t length;
} Buffer;

/**
 * Sets a \param field with the memory pointed to by a \param ptr
 * using memcpy(), which circumvents const modifiers.
 *
 * @param field the field to set with the memory pointed to by \param ptr
 * @param ptr the pointer to the memory to set the \param field with
 */
#define set_field_memory(field, ptr) \
    memcpy((void *) &(field), ptr, sizeof(field))

/**
 * Sets a \param field with a \param val using memcpy(),
 * which circumvents const modifiers.
 *
 * @param field the field to set with \param val
 * @param val the value to set the \param field with
 */
#define set_field(field, val) \
    do { \
        typeof(val) _val = val; \
        set_field_memory(field, &_val); \
    } while (false)

/**
 * Puts a value into this buffer and advances the buffer's index.
 *
 * @param buffer the buffer the \param val is being added to
 * @param val the value being added to the \param buffer
 */
#define Buffer_put(buffer, val) \
    memcpy((buffer)->data + (buffer)->index, &(val), sizeof(val)); \
    (buffer)->index += sizeof(val)

#define array_size(array, length) ((length) * sizeof(*(array)))

/**
 * Puts a typed array of length \param length into this buffer
 * and advances the buffer's index.
 *
 * Note: All lengths are serialized as size_t,
 * no matter what its original type is.
 *
 * @param buffer the buffer the \param array is being added to
 * @param array the typed array being added to the \param buffer
 * @param length the length of the array in terms of its type
 */
#define Buffer_put_array(buffer, array, length) \
    do { \
        const size_t size = array_size(array, length); \
        Buffer_put(buffer, size); \
        memcpy((buffer)->data + (buffer)->index, array, size); \
        (buffer)->index += size; \
    } while (false)

/**
 * Gets a value of type \param var from a \param buffer
 * and places it in a \param var.
 * Also advances the \param buffer's index.
 *
 * @param buffer the buffer to get the value from
 * @param var the variable to place the value in
 */
#define Buffer_get(buffer, var) \
    memcpy((void *) &(var), (buffer)->data + (buffer)->index, sizeof(var)); \
    (buffer)->index += sizeof(var)

/**
 * Gets an array serialized in a \param buffer
 * and places it (reallocing) into an \param array
 * and sets \param length_var to the array's length.
 * Also advances the \param buffer's index.
 *
 * Note: All lengths are serialized as size_t,
 * no matter what its original type is.
 *
 * @param buffer the buffer to get the array from
 * @param array a pointer to an array that will be realloc'ed to store the array data from the buffer
 * @param length_var an integer variable that will store the length of the array w.r.t to its type
 */
#define Buffer_get_array(buffer, array, length_var) \
    do { \
        size_t length; \
        Buffer_get(buffer, length); \
        typeof(array) alloced_array = realloc(array, length); \
        memcpy(alloced_array, (buffer)->data + (buffer)->index, array_size(array, length)); \
        (buffer)->index += array_size(array, length); \
        memcpy((void *) &(array), &alloced_array, sizeof(array)); \
        typeof(length_var) casted_length = (typeof(length_var)) (length / sizeof(*(array))); \
        memcpy((void *) &(length_var), &casted_length, sizeof(casted_length)); \
    } while (false)

extern const Buffer INVALID_BUFFER;

#endif // CASTROPARTY_BUFFER_H
