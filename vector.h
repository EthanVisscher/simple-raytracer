#ifndef VECTOR_H
#define VECTOR_H

#include <math.h>

typedef struct {
    float x;
    float y;
    float z;
} Coordinates3D;

void normalize_vector(Coordinates3D *vector_ptr);
float dot_product(Coordinates3D vector_a, Coordinates3D vector_b);
Coordinates3D cross_product(Coordinates3D vector_a, Coordinates3D vector_b);
Coordinates3D vector_add(Coordinates3D vector_a, Coordinates3D vector_b);
Coordinates3D vector_subtract(Coordinates3D minuend_vector, Coordinates3D subtrahend_vector);
Coordinates3D vector_multiply_scalar(Coordinates3D vector, float scalar);

#endif