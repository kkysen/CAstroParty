//
// Created by kkyse on 12/30/2017.
//

#ifndef CASTROPARTY_GAME_PACKET_H
#define CASTROPARTY_GAME_PACKET_H

#include <stdint.h>

#include "reaction.h"

typedef struct {
    const Reaction reaction;
    const uint64_t hash;
} GamePacket;

#endif // CASTROPARTY_GAME_PACKET_H
