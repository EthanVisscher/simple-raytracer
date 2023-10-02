#ifndef RAYTRACER_H
#define RAYTRACER_H

#include <stdlib.h>
#include <stdint.h>
#include "vector.h"
#include <math.h>
#include "image_utils.h"

#define RAYHIT_SPHERE   1
#define RAYHIT_TRIANGLE 2

typedef struct {
    float red;
    float green;
    float blue;
    uint16_t reflective;
} Material;

typedef struct {
    Coordinates3D position;
    float radius;
    Material material;
} Sphere;

typedef struct {
    Coordinates3D vertex1;
    Coordinates3D vertex2;
    Coordinates3D vertex3;
    Material material;
} Triangle;

typedef struct {
    Sphere  *spheres;
    Triangle *triangles;
    uint16_t spheres_length;
    uint16_t triangles_length;
} Objects;

typedef struct {
    Coordinates3D position;
    Coordinates3D direction;
} Ray;

typedef struct {
    float t;
    Coordinates3D intersect_position;
    Material material;
    uint8_t type;
    Sphere sphere;
    Triangle triangle;
} Rayhit;

typedef struct {
    float world_width;
    float world_height;
    uint16_t pixel_width;
    uint16_t pixel_height;
    Coordinates3D position;
} ImagePlane;

Ray get_ray(Coordinates3D camera_position, Coordinates2D pixel_position, ImagePlane image_plane);
Rayhit ray_sphere_intersect(Ray ray, Sphere sphere);
Rayhit ray_triangle_intersect(Ray ray, Triangle triangle);
Rayhit ray_intersect(Ray ray, Objects objects);
Rayhit reflect(Ray ray, Rayhit rayhit, Objects objects);
float diffuse_hit(Rayhit rayhit, Coordinates3D light_position, Objects objects);
Coordinates3D get_surface_normal(Rayhit rayhit);

#endif