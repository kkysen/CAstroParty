#ifndef VECTOR_H
#define VECTOR_H

#include <math.h>

struct vector{
	float x;
	float y;
} typedef Vector;

Vector Vector_new(float x, float y){
	return (Vector){.x = x, .y = y};
}

float Vector_magnitude(Vector vector){
	return (float)sqrt(pow(vector.x, 2) + pow(vector.y, 2));
}

Vector Vector_scale(Vector vector, float scalar){
	return (Vector){.x = vector.x * scalar, .y = vector.y * scalar};
}

Vector Vector_normalize(Vector vector){
	return scale(vector, 1/magnitude(vector));
}

Vector Vector_add(Vector vector1, Vector vector2){
	return (Vector){.x = vector1.x + vector2.x, .y = vector1.y + vector2.y};
}

#endif