#include <stdbool.h>


#ifndef D_NETWORKING
#define D_NETWORKING

// Packet that's constructed after receiving raw data
// NOTE: This struct is NOT sent. A char is sent.
struct Networking_unpacked_inputs {
    bool is_accelerating;
    bool is_turning_left;
    bool is_shooting;
};

char Networking_compress_inputs(bool is_accelerating, bool is_turning_left, bool is_shooting);

struct Networking_unpacked_inputs *Networking_decompress_inputs(char data);

#endif
