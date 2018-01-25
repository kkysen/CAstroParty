//
// Created by kkyse on 1/24/2018.
//

#ifndef CASTROPARTY_SERIALIZATION_H
#define CASTROPARTY_SERIALIZATION_H

#include "../networking.h"

/**
 * Defines a serializer function for the given \param Type.
 * It requires that these functions already be defined:
 *      size_t Type##_serialized_length(Type type);
 *      void Type##_serialize_into(Type type, Buffer *buffer);
 *
 * @param Type the type of value for which the serializer should be defined.
 */
#define define_serializer(Type) \
Buffer Type##_serialize(Type *const type) { \
    const size_t size = Type##_serialized_length(type); \
    Buffer buffer = { \
            .index = 0, \
            .length = size, \
            .data = malloc(size), \
    }; \
    if (!buffer.data) { \
        perror("malloc(size)"); \
        return INVALID_BUFFER; \
    } \
    Type##_serialize_into(type, &buffer); \
    return buffer; \
}


#endif // CASTROPARTY_SERIALIZATION_H
