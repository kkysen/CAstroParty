//
// Created by kkyse on 12/25/2017.
//

#include "sdl_utils.h"

void _sdl_log_func(FILE *const file, int (*const func)(), const char *const func_str, const int least_ret_val) {
    const char *const prev_error = str_copy(SDL_GetError());
    const int value = func();
    if (value < least_ret_val) {
        sdl_perror(func_str);
        SDL_SetError("%s", prev_error);
    }
    free((char *) prev_error);
    fprintf(file, "%s: %d\n\n", func_str, value);
}