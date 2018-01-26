//
// Created by kkyse on 12/25/2017.
//

#ifndef CASTROPARTY_BULLET_H
#define CASTROPARTY_BULLET_H

#include "player.h"

typedef struct {
    Player *const owner;
    Vector position;
    const Vector velocity;
} Bullet;

#endif // CASTROPARTY_BULLET_H
