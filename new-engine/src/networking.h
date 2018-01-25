#include <stdbool.h>


#ifndef D_NETWORKING
#define D_NETWORKING

// This is sent first, and lets the client know
// what kind of data it's receiving.
#define SERVER_NETWORKING_INPUT_ID 1
#define SERVER_NETWORKING_DATA_ID 2

// Packet that's constructed after receiving raw data
// NOTE: This struct is NOT sent. A char is sent.
struct Networking_unpacked_inputs {
    bool is_accelerating;
    bool is_turning_left;
    bool is_shooting;
};

// Packet containing the data of a single player
struct Networking_unpacked_player {
    float x,y,angle;
    //index is handled by its position in the array... int server_index;
};

char Networking_compress_inputs(bool is_accelerating, bool is_turning_left, bool is_shooting);

struct Networking_unpacked_inputs *Networking_decompress_inputs(char data);

#endif
