#include <stdbool.h>


#ifndef D_NETWORKING
#define D_NETWORKING

// Packet that's constructed after receiving raw data
// NOTE: This struct is NOT sent. A char is sent.
struct Networking_unpacked_inputs {
    bool is_turning;
    bool is_shooting;
    bool restart;
};

char Networking_compress_inputs(bool is_turning, bool is_shooting, bool restart);

struct Networking_unpacked_inputs *Networking_decompress_inputs(char data);

#endif
