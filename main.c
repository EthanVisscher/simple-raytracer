#include "raytracer.h"
#include "image_utils.h"
#include "vector.h"
#include <stdio.h>

// Helper function to print ray information
void print_ray(Ray ray) {
    printf("RayPosition %f %f %f\n", ray.position.x, ray.position.y, ray.position.z);
    printf("RayDirection %f %f %f\n\n", ray.direction.x, ray.direction.y, ray.direction.z);
}

int main() {
    ImagePlane image_plane = {
        .world_width = 2.0,
        .world_height = 2.0,
        .pixel_width = 512,
        .pixel_height = 512,
        .position.x = 0,
        .position.y = 0,
        .position.z = -2.0
    };

    Image image = get_image("reference.png", image_plane.pixel_width, image_plane.pixel_height);

    Coordinates3D camera_position = {
        .x = 0.0,
        .y = 0.0,
        .z = 0.0
    };

    Coordinates3D light_position = {
        .x =  3,
        .y = 5,
        .z = -15
    };

    Material reflection = {
        .red = 0,
        .green = 0,
        .blue = 0,
        .reflective = 1
    };

    Material red = {
        .red = 1,
        .green = 0,
        .blue = 0,
        .reflective = 0
    };

    Material blue = {
        .red = 0,
        .green = 0,
        .blue = 1,
        .reflective = 0
    };

    Material white = {
        .red = 1,
        .green = 1,
        .blue = 1,
        .reflective = 0
    };


    Objects objects = {
        .spheres = (Sphere *)malloc(3 * sizeof(Sphere)),
        .triangles = (Triangle *)malloc(5 * sizeof(Triangle)),
        .spheres_length = 3,
        .triangles_length = 5 
    };

    objects.spheres[0] = (Sphere){
        .position.x = 0,
        .position.y = 0,
        .position.z = -16,
        .radius = 2,
        .material = reflection
    };

    objects.spheres[1] = (Sphere){
        .position.x = 3,
        .position.y = -1,
        .position.z = -14,
        .radius = 1,
        .material = reflection
    };

    objects.spheres[2] = (Sphere){
        .position.x = -3,
        .position.y = -1,
        .position.z = -14,
        .radius = 1,
        .material = red
    };

    objects.triangles[0] = (Triangle){
        .vertex1.x = -8,
        .vertex1.y = -2,
        .vertex1.z = -20,

        .vertex2.x = 8,
        .vertex2.y = -2,
        .vertex2.z = -20,

        .vertex3.x = 8,
        .vertex3.y = 10,
        .vertex3.z = -20,

        .material = blue
    }; 

    objects.triangles[1] = (Triangle){
        .vertex1.x = -8,
        .vertex1.y = -2,
        .vertex1.z = -20,

        .vertex2.x = 8,
        .vertex2.y = 10,
        .vertex2.z = -20,

        .vertex3.x = -8,
        .vertex3.y = 10,
        .vertex3.z = -20,

        .material = blue
    }; 

    objects.triangles[2] = (Triangle){
        .vertex1.x = -8,
        .vertex1.y = -2,
        .vertex1.z = -20,

        .vertex2.x = 8,
        .vertex2.y = -2,
        .vertex2.z = -10,

        .vertex3.x = 8,
        .vertex3.y = -2,
        .vertex3.z = -20,

        .material = white
    }; 

    objects.triangles[3] = (Triangle){
        .vertex1.x = -8,
        .vertex1.y = -2,
        .vertex1.z = -20,

        .vertex2.x = -8,
        .vertex2.y = -2,
        .vertex2.z = -10,

        .vertex3.x = 8,
        .vertex3.y = -2,
        .vertex3.z = -10,

        .material = white
    }; 

    objects.triangles[4] = (Triangle){
        .vertex1.x = 8,
        .vertex1.y = -2,
        .vertex1.z = -20,

        .vertex2.x = 8,
        .vertex2.y = -2,
        .vertex2.z = -10,

        .vertex3.x = 8,
        .vertex3.y = 10,
        .vertex3.z = -20,

        .material = red
    }; 

    // Shoot ray out of camera for every pixel in image, check intersections, reflect and diffuse hits
    for (int y = 0; y < image_plane.pixel_height; y++) {
        for (int x = 0; x < image_plane.pixel_width; x++) {
            Pixel pixel;
            pixel.position.x = x;
            pixel.position.y = y;

            Ray ray = get_ray(camera_position, pixel.position, image_plane); 
            Rayhit closest_hit = ray_intersect(ray, objects);
            closest_hit = reflect(ray, closest_hit, objects);
            float diffuse = diffuse_hit(closest_hit, light_position, objects);

            if (closest_hit.t != -1) {
                pixel.red = 255 * diffuse * closest_hit.material.red;
                pixel.green = 255 * diffuse * closest_hit.material.green;
                pixel.blue = 255 * diffuse * closest_hit.material.blue;
            } else {
                pixel.red = 0;
                pixel.green = 0;
                pixel.blue = 0;
            }

            fill_pixel_on_image(&image, pixel);
        }
    }

    render_image(image);
    free_image(&image);
    free(objects.spheres);
    free(objects.triangles);
}