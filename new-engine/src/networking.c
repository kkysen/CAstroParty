/** networking.c
 *      Handles networking
 *
 *      For now, we're just swapping keyboard inputs.
 *
 *      The clients send their keyboard inputs to the server
 *
 *      The server sends them back to each client, and the
 *      client simulates its own game after unpacking the
 *      given data
 */

#include "networking.h"
#include <stdbool.h>
#include <stdlib.h>

// Sets the nth bit of a char
void util_set_nth_bit(char *value, char index, bool setto) {
    *value |= (setto << index);
}

// Reads the nth bit of a char
char util_read_nth_bit(char value, char index) {
    return (value >> index) & 1;
}

/* Networking_compress_inputs( is_accelerating, is_turning_left, is_shooting )
 *      Compresses our input data into a char, which we can send via networking
 */
char Networking_compress_inputs(bool is_accelerating, bool is_turning_left, bool is_shooting) {
    char result = 0;
    util_set_nth_bit(&result, 0, is_accelerating);
    util_set_nth_bit(&result, 1, is_turning_left);
    util_set_nth_bit(&result, 2, is_shooting);
    return result;
}

/* Networking_decompress_inputs( data )
 *      Decompress our input data into a readable struct
 */
struct Networking_unpacked_inputs *Networking_decompress_inputs(char data) {
    struct Networking_unpacked_inputs *unpacked = malloc(sizeof(struct Networking_unpacked_inputs));

    unpacked->is_accelerating = util_read_nth_bit(data, 0);
    unpacked->is_turning_left = util_read_nth_bit(data, 1);
    unpacked->is_shooting     = util_read_nth_bit(data, 2);

    return unpacked;
}
