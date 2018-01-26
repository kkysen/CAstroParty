//
// Created by kkyse on 12/13/2017.
//

#include "random.h"

#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

typedef unsigned int uint;

bool securely_seeded_rand = false;

int securely_seed_rand() {
    if (securely_seeded_rand) {
        return 0;
    }
    const int fd = open("/dev/urandom", O_RDONLY);
    if (fd == -1) {
        return -1;
    }
    uint seed;
    if (read(fd, &seed, sizeof(seed)) != sizeof(seed)) {
        return -1;
    }
    srand(seed);
    securely_seeded_rand = true;
    return 0;
}

char random_lowercase_letter() {
    return (char) (rand() % ('z' - 'a' + 1) + 'a');
}

int random_lowercase_letters(char *const buf, size_t count) {
    if (securely_seed_rand() == -1) {
        return -1;
    }
    while (count-- > 0) {
        buf[count] = random_lowercase_letter();
    }
    return 0;
}
