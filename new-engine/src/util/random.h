//
// Created by kkyse on 12/13/2017.
//

#ifndef SYSTEMS_RANDOM_H
#define SYSTEMS_RANDOM_H

#include <stdbool.h>
#include <stdlib.h>

extern bool securely_seeded_rand;

int securely_seed_rand();

char random_lowercase_letter();

int random_lowercase_letters(char *buf, size_t count);

#endif //SYSTEMS_RANDOM_H
