#ifndef CASTROPARTY_VECTOR_H
#define CASTROPARTY_VECTOR_H

#include <math.h>
#include <stdint.h>

typedef union {
    struct {
        float x;
        float y;
    };
    uint64_t bits;
} Vector;

#define deg2rad(degrees) ((degrees) * (float) M_PI / 180)

#define rad2deg(radians) ((radians) * 180 / (float) M_PI)

#define Vector_new(_x, _y) (Vector) {.x = (_x), .y = (_y)}

#define Vector_norm2(vector) ((vector).x * (vector).x + (vector).y * (vector).y)

#define Vector_magnitude2(vector) Vector_norm2(vector)

#define Vector_norm(vector) sqrtf(Vector_norm2(vector))

#define Vector_magnitude(vector) Vector_norm(vector)

#define Vector_i_scale(vector, scalar) { \
    const float _scalar = scalar; \
    (vector).x *= _scalar; \
    (vector).y *= _scalar; \
}

Vector Vector_scale(Vector vector, float scalar);

#define Vector_i_normalize(vector) Vector_i_scale(vector, 1 / Vector_norm(vector))

Vector Vector_normalize(Vector vector);

#define Vector_i_add(vector1, vector2) {(vector1).x += (vector2).x; (vector1).y += (vector2).y;}

#define Vector_i_mul_add(vector1, vector2, scalar) \
{ \
    (vector1).x += (vector2).x * (scalar); \
    (vector1).y += (vector2).y * (scalar); \
}

Vector Vector_add(Vector vector1, Vector vector2);

float Vector_dist2(Vector vector1, Vector vector2);

#define Vector_dist(vector1, vector2) sqrtf(Vector_dist2(vector1, vector2))

#define Vector_in_radius(vector1, vector2, radius) Vector_dist(vector1, vector2) < (radius) * (radius)

#define Vector_as_SDL_Point(vector) ((SDL_Point) {.x = (vector).x, .y = (vector).y})

#define clamped_min(val, min) \
    (fmaxf(val, min))

#define clamped_max(val, max) \
    (fminf(val, max))

#define clamp_min(val, min) \
    (val) = clamped_min(val, min)

#define clamp_max(val, max) \
    (val) = clamped_max(val, max)

#define clamp(val, min, max) \
    clamp_min(val, min); \
    clamp_max(val, max)

#define Vector_clamp(vector, min, max) \
    clamp((vector).x, (min).x, (max).x); \
    clamp((vector).y, (min).y, (max).y)

#define pv(vector) printf(#vector": (%f, %f)\n", (vector).x, (vector).y)

#endif // CASTROPARTY_VECTOR_H