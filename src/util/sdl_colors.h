//
// Created by kkyse on 12/25/2017.
//

#ifndef CASTROPARTY_SDL_COLORS_H
#define CASTROPARTY_SDL_COLORS_H

#include <SDL2/SDL_pixels.h>

#define rgba(_r, _g, _b, _a) (SDL_Color) {.r = (_r), .g = (_g), .b = (_b), .a = (_a)}
#define rgb(r, g, b) rgba(r, g, b, 255)

#define rgba_args(rgba) (rgba).r, (rgba).g, (rgba).b, (rgba).a

#define extern_rgb(name) extern const SDL_Color name;

extern_rgb(BLACK);
extern_rgb(WHITE);

extern_rgb(RED);
extern_rgb(GREEN);
extern_rgb(BLUE);
extern_rgb(YELLOW);
extern_rgb(CYAN);
extern_rgb(MAGENTA);

extern_rgb(GRAY);
extern_rgb(SILVER);

extern_rgb(MAROON);
extern_rgb(OLIVE);
extern_rgb(DARK_GREEN);
extern_rgb(PURPLE);
extern_rgb(TEAL);
extern_rgb(NAVY);

#undef extern_rgb

uint32_t SDL_Color_bits(SDL_Color color);

#endif //CASTROPARTY_SDL_COLORS_H
