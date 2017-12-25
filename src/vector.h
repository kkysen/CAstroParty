#ifndef CASTROPARTY_VECTOR_H
#define CASTROPARTY_VECTOR_H

#include <math.h>

typedef struct {
    float x;
    float y;
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

inline Vector Vector_scale(Vector vector, float scalar);

#define Vector_i_normalize(vector) Vector_i_scale(vector, 1 / Vector_norm(vector))

inline Vector Vector_normalize(Vector vector);

#define Vector_i_add(vector1, vector2) {(vector1).x += (vector2).x; (vector1).y += (vector2).y;}

inline Vector Vector_add(Vector vector1, Vector vector2);

inline float Vector_dist2(Vector vector1, Vector vector2);

#define Vector_dist(vector1, vector2) sqrtf(Vector_dist2(vector1, vector2))

#define Vector_in_radius(vector1, vector2, radius) Vector_dist(vector1, vector2) < (radius) * (radius)

#endif // CASTROPARTY_VECTOR_H