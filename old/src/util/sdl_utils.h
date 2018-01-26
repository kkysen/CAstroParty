//
// Created by kkyse on 12/25/2017.
//

#ifndef CASTROPARTY_SDL_UTILS_H
#define CASTROPARTY_SDL_UTILS_H

#include <SDL2/SDL_quit.h>

#include "utils.h"
#include "string_utils.h"

#define sdl_perror(s) perror(s), _perror(SDL_GetError(), s)

#define _sdl_check(cmp, expr) if ((expr) cmp) return -1

#define _sdl_check_perror(cmp, expr) if ((expr) cmp) return sdl_perror(#expr), -1

#define _sdl_check_perror_msg(cmp, expr, msg) if ((expr) cmp) return sdl_perror(msg), -1

#define _sdl_warn_perror(cmp, expr) if ((expr) cmp) sdl_perror(#expr)

#define _sdl_warn_perror_msg(cmp, expr, msg) if ((expr) cmp) sdl_perror(msg)

// ==========================================

#define sdl_check(expr) _sdl_check(< 0, expr)

#define sdl_check_perror(expr) _sdl_check_perror(< 0, expr)

#define sdl_check_perror_msg(expr, msg) _sdl_check_perror_msg(< 0, expr, msg)

#define sdl_check_null(expr) _sdl_check(== NULL, expr)

#define sdl_check_null_perror(expr) _sdl_check_perror(== NULL, expr)

#define sdl_check_null_perror_msg(expr, msg) _sdl_check_perror_msg(== NULL, expr, msg)

#define sdl_warn_perror(expr) _sdl_warn_perror(< 0, expr)

#define sdl_warn_null_perror(expr) _sdl_warn_perror(== NULL, expr)

// ==========================================

void _sdl_log_func(FILE *file, int (*func)(), const char *func_str, int least_ret_val);

#define sdl_log_func(file, func_call, least) \
{ \
    const char *const prev_error = str_copy(SDL_GetError()); \
    const int value = func_call; \
    if (value < (least)) { \
        sdl_perror(#func_call); \
        SDL_SetError("%s", prev_error); \
    } \
    free((char *) prev_error); \
    fprintf(file, "%s: %d\n\n", #func_call, value); \
}

#endif // CASTROPARTY_SDL_UTILS_H
