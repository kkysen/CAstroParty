//
// Created by kkyse on 12/25/2017.
//

#include "sdl_colors.h"

#define def_rgb(name, r, g, b) const SDL_Color name = rgb(r, g, b)

def_rgb(BLACK, 0, 0, 0);
def_rgb(WHITE, 255, 255, 255);

def_rgb(RED, 255, 0, 0);
def_rgb(GREEN, 0, 255, 0);
def_rgb(BLUE, 0, 0, 255);
def_rgb(YELLOW, 255, 255, 0);
def_rgb(MAGENTA, 255, 0, 255);
def_rgb(CYAN, 0, 255, 255);

def_rgb(GRAY, 128, 128, 128);
def_rgb(SILVER, 192, 192, 192);

def_rgb(MAROON, 128, 0, 0);
def_rgb(DARK_GREEN, 0, 128, 0);
def_rgb(NAVY, 0, 0, 128);
def_rgb(OLIVE, 128, 128, 0);
def_rgb(PURPLE, 128, 0, 128);
def_rgb(TEAL, 0, 128, 128);

uint32_t SDL_Color_bits(const SDL_Color color) {
    return (color.r << 0) & (color.g << 8) & (color.b << 16) & (color.a << 24);
}

#undef def_rgb
