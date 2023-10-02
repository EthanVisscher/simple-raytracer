#include "raytracer.h"

/**
 * Return ray structure shooting from camera position to the pixel on the image plane
*/
Ray get_ray(Coordinates3D start_position, Coordinates2D pixel_position, ImagePlane image_plane) {
    Ray ray;

    ray.position.x = start_position.x;
    ray.position.y = start_position.y;
    ray.position.z = start_position.z;

    ray.direction.x = (pixel_position.x + 0.5) * (image_plane.world_width / image_plane.pixel_width) - 1;
    ray.direction.y = 1 - (pixel_position.y + 0.5) * (image_plane.world_height / image_plane.pixel_height);
    ray.direction.z = image_plane.position.z;


    normalize_vector(&ray.direction);
    return ray;
}

/**
 * Calculates if the given ray intersects with the given sphere.
 * t value will be >= 0 if there is an intersection.
*/
Rayhit ray_sphere_intersect(Ray ray, Sphere sphere) {
    Rayhit rayhit = {
        .t = -1,
        .material = sphere.material,
        .type = RAYHIT_SPHERE,
        .sphere = sphere
    };

    // Sphere-ray intersection variables 
    Coordinates3D d = ray.direction;
    Coordinates3D e = ray.position;
    Coordinates3D c = sphere.position;
    float R = sphere.radius;

    float quad_a = dot_product(d, d);
    float quad_b = dot_product(d, vector_subtract(e, c));
    float quad_c = dot_product(vector_subtract(e, c), vector_subtract(e, c)) - R * R;
    float t = ((quad_b * quad_b) - (quad_a * quad_c));  // determinent of quadratic equation

    if (t < 0) {
        return rayhit;  // No hit
    } 

    // We want the intersection closest to the camera, not the intersection on the back of the sphere
    if (t > 0) {
        float t_plus = (-quad_b + sqrt(t)) / (dot_product(d, d));
        float t_minus = (-quad_b - sqrt(t)) / (dot_product(d, d));

        if (t_plus < t_minus) {
            t = t_plus;
        } else {
            t = t_minus;
        }
    }

    // Hit

    // Prevents static/acne
    if (t < 0.001) {
        t = -1;
    }

    rayhit.t = t;
    rayhit.intersect_position = vector_add(e, vector_multiply_scalar(d, t));
    return rayhit;
}

/**
 * Calculates if the given ray intersects with the given triangle.
 * t value will be >= 0 if there is an intersection.
*/
Rayhit ray_triangle_intersect(Ray ray, Triangle triangle) {
    Rayhit rayhit = {
        .t = -1,
        .material = triangle.material,
        .type = RAYHIT_TRIANGLE,
        .triangle = triangle
    };

    // Triangle-ray intersection equation variables
    float A = triangle.vertex1.x - triangle.vertex2.x;
    float B = triangle.vertex1.y - triangle.vertex2.y;
    float C = triangle.vertex1.z - triangle.vertex2.z;

    float D = triangle.vertex1.x - triangle.vertex3.x;
    float E = triangle.vertex1.y - triangle.vertex3.y;
    float F = triangle.vertex1.z - triangle.vertex3.z;

    float G = ray.direction.x;
    float H = ray.direction.y;
    float I = ray.direction.z;

    float J = triangle.vertex1.x - ray.position.x;
    float K = triangle.vertex1.y - ray.position.y;
    float L = triangle.vertex1.z - ray.position.z;

    float EI_minus_HF = (E * I) - (H * F);
    float GF_minus_DI = (G * F) - (D * I);
    float DH_minus_EG = (D * H) - (E * G);

    float AK_minus_JB = (A * K) - (J * B);
    float JC_minus_AL = (J * C) - (A * L);
    float BL_minus_KC = (B * L) - (K * C);

    float M = (A * EI_minus_HF) + (B * GF_minus_DI) + (C * DH_minus_EG);

    float t = -((F * AK_minus_JB) + (E * JC_minus_AL) + (D * BL_minus_KC)) / M;
    if (t < 0) {
        return rayhit;  // No hit
    }

    float gamma = ((I * AK_minus_JB) + (H * JC_minus_AL) + (G * BL_minus_KC)) / M;
    if (gamma < 0 || gamma > 1) {
        return rayhit;  // No hit
    }

    float beta = ((J * EI_minus_HF) + (K * GF_minus_DI) + (L * DH_minus_EG)) / M;
    if (beta < 0 || beta > (1 - gamma)) {
        return rayhit;  // No hit
    }

    // Hit

    // Prevents static/acne
    if (t < 0.001) {
        t = -1;
    }

    rayhit.t = t;
    rayhit.intersect_position = vector_add(ray.position, vector_multiply_scalar(ray.direction, t));
    return rayhit;
}

/**
 * Iterate through all objects checking closest intersection for the given ray
*/
Rayhit ray_intersect(Ray ray, Objects objects) {
    uint8_t hit = 0;
    Rayhit closest_hit = { .t = -1 };
    for (int i = 0; i < objects.spheres_length; i++) {
        Rayhit rayhit = ray_sphere_intersect(ray, objects.spheres[i]);
        if (rayhit.t >= 0) {
            if (hit == 0) {
                closest_hit = rayhit;
                hit = 1;
            } else if (rayhit.t < closest_hit.t) {
                closest_hit = rayhit;
            }
        }
    }

    for (int i = 0; i < objects.triangles_length; i++) {
        Rayhit rayhit = ray_triangle_intersect(ray, objects.triangles[i]);
        if (rayhit.t >= 0) {
            if (hit == 0) {
                closest_hit = rayhit;
                hit = 1;
            } else if (rayhit.t < closest_hit.t) {
                closest_hit = rayhit;
            }
        }
    }

    return closest_hit;
}

/**
 * For a given ray and rayhit, check if the ray reflects off the object
*/
Rayhit reflect(Ray ray, Rayhit rayhit, Objects objects) {
    if (rayhit.t == -1 || rayhit.material.reflective != 1) {
        return rayhit;
    }

    // Max 10 times for bouncing
    for (int i = 0; i < 10; i++) {
        Coordinates3D surface_normal = get_surface_normal(rayhit);
        Coordinates3D new_direction = vector_subtract(ray.direction, 
                                      vector_multiply_scalar(surface_normal, 2 * dot_product(ray.direction, surface_normal))); 

        ray.position = rayhit.intersect_position;
        ray.direction = new_direction;

        rayhit = ray_intersect(ray, objects); 
        if (rayhit.t == -1 || rayhit.material.reflective == 0) {
            return rayhit;
        }
    }

    // No hit
    rayhit.t = -1;
    return rayhit;
}

/**
 * Calculate diffuse value for given rayhit, also checks if a shadow is cast
*/
float diffuse_hit(Rayhit rayhit, Coordinates3D light_position, Objects objects) {
    if (rayhit.t == -1 || rayhit.material.reflective != 0) {
        return 1.0;
    }

    float diffuse = 0.0;

    Coordinates3D to_light = vector_subtract(light_position, rayhit.intersect_position);
    normalize_vector(&to_light);

    Coordinates3D surface_normal = get_surface_normal(rayhit);

    Ray ray = {
        .direction = to_light,
        .position = rayhit.intersect_position
    };

    rayhit = ray_intersect(ray, objects);

    if (rayhit.t != -1) {
        diffuse = 0.2;  // Can't see light, cast shadow
    } else {
        diffuse = dot_product(surface_normal, to_light);
        if (diffuse < 0.2) {
            diffuse = 0.2;
        }
    }

    return diffuse;
}

/**
 * Returns surface normal for rayhit
*/
Coordinates3D get_surface_normal(Rayhit rayhit) {
    Coordinates3D surface_normal;

    if (rayhit.type == RAYHIT_SPHERE) {
        surface_normal = vector_subtract(rayhit.intersect_position, rayhit.sphere.position);
    } else {
        Coordinates3D vector_a = vector_subtract(rayhit.triangle.vertex1, rayhit.triangle.vertex2);
        Coordinates3D vector_b = vector_subtract(rayhit.triangle.vertex1, rayhit.triangle.vertex3);
        surface_normal = cross_product(vector_a, vector_b);
    }
    normalize_vector(&surface_normal);
    return surface_normal;
}