//
// Created by kkyse on 11/9/2017.
//

#include "utils.h"

#include <stdlib.h>
#include <math.h>
#include <time.h>

const char *HOME = NULL;

void init_home() {
    HOME = getenv("HOME");
}

void double_sleep(const double seconds) {
    struct timespec timespec = {
            .tv_sec = (time_t) seconds,
            .tv_nsec = (long) ((seconds - floor(seconds)) * 1e9)
    };
    nanosleep(&timespec, NULL);
}