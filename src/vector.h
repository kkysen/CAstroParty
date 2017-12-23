#ifndef VECTOR_H
#define VECTOR_H

#include <math.h>

struct vector{
	float x;
	float y;
};

struct vector new_vector(float x, float y){
	return (struct vector){.x = x, .y = y};
}

float magnitude(struct vector vector){
	return (float)sqrt(pow(vector.x, 2) + pow(vector.y, 2));
}

struct vector scale(struct vector vector, float scalar){
	return (struct vector){.x = vector.x * scalar, .y = vector.y * scalar};
}

struct vector normalize(struct vector vector){
	return scale(vector, 1/magnitude(vector));
}

struct vector vector_add(struct vector vector1, struct vector vector2){
	return (struct vector){.x = vector1.x + vector2.x, .y = vector1.y + vector2.y};
}

#endif