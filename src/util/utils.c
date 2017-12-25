//
// Created by kkyse on 11/9/2017.
//

#include "utils.h"

#include <stdlib.h>

const char *HOME = NULL;

void init_home() {
    HOME = getenv("HOME");
}