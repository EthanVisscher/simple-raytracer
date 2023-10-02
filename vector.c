#include "vector.h"

/**
 * Normalizes the given vector
*/
void normalize_vector(Coordinates3D *vector_ptr) {
    float x_direction = vector_ptr->x;
    float y_direction = vector_ptr->y;
    float z_direction = vector_ptr->z;

    float magnitude = sqrt(x_direction * x_direction +
                           y_direction * y_direction +
                           z_direction * z_direction);
    
    vector_ptr->x = x_direction / magnitude;
    vector_ptr->y = y_direction / magnitude;
    vector_ptr->z = z_direction / magnitude;
}

/**
 * Calculates the dot product of the two given vectors
*/
float dot_product(Coordinates3D vector_a, Coordinates3D vector_b) {
    return vector_a.x * vector_b.x + vector_a.y * vector_b.y + vector_a.z * vector_b.z;
}

/**
 * Calculates the cross product of the two given vectors, returns new vector
*/
Coordinates3D cross_product(Coordinates3D vector_a, Coordinates3D vector_b) {
    Coordinates3D vector;

    vector.x = (vector_a.y * vector_b.z) - (vector_a.z * vector_b.y);
    vector.y = (vector_a.z * vector_b.x) - (vector_a.x * vector_b.z);
    vector.z = (vector_a.x * vector_b.y) - (vector_a.y * vector_b.x);

    return vector;
}

/**
 * Adds the two vectors, returns new vector
*/
Coordinates3D vector_add(Coordinates3D vector_a, Coordinates3D vector_b) {
    vector_a.x += vector_b.x;
    vector_a.y += vector_b.y;
    vector_a.z += vector_b.z;
    return vector_a;
}

/**
 * Subtracts the subtrahend vector from the minuend vector, returns new vector
*/
Coordinates3D vector_subtract(Coordinates3D minuend_vector, Coordinates3D subtrahend_vector) {
    minuend_vector.x -= subtrahend_vector.x;
    minuend_vector.y -= subtrahend_vector.y;
    minuend_vector.z -= subtrahend_vector.z;
    return minuend_vector;
}

/**
 * Multiplies the vector and scalar, returns new vector
*/
Coordinates3D vector_multiply_scalar(Coordinates3D vector, float scalar) {
    vector.x *= scalar;
    vector.y *= scalar;
    vector.z *= scalar;
    return vector;
}