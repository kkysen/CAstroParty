//
// Created by kkyse on 12/25/2017.
//

#ifndef CASTROPARTY_SDL_UTILS_H
#define CASTROPARTY_SDL_UTILS_H

#include <SDL2/SDL_quit.h>

#include "utils.h"

#define sdl_perror(s) perror(s), fprintf(stderr, "[%d] %s: %s\n", getpid(), s, SDL_GetError())

#define _sdl_check(cmp, expr) if ((expr) cmp) return -1

#define _sdl_check_perror(cmp, expr) if ((expr) cmp) return sdl_perror(#expr), -1

#define _sdl_check_perror_msg(cmp, expr, msg) if ((expr) cmp) return sdl_perror(msg), -1

#define sdl_check(expr) _sdl_check(< 0, expr)

#define sdl_check_perror(expr) _sdl_check_perror(< 0, expr)

#define sdl_check_perror_msg(expr, msg) _sdl_check_perror_msg(< 0, expr, msg)

#define sdl_check_null(expr) _sdl_check(== NULL, expr)

#define sdl_check_null_perror(expr) _sdl_check_perror(== NULL, expr)

#define sdl_check_null_perror_msg(expr, msg) _sdl_check_perror_msg(== NULL, expr, msg)

#endif // CASTROPARTY_SDL_UTILS_H
