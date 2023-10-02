#ifndef IMAGE_UTILS_H
#define IMAGE_UTILS_H

#include <stdint.h>

#define PIXEL_CHANNELS          3
#define RED_CHANNEL_OFFSET      0
#define GREEN_CHANNEL_OFFSET    1
#define BLUE_CHANNEL_OFFSET     2

// Coordinate structures
typedef struct {
    uint16_t x;
    uint16_t y;
} Coordinates2D;

typedef struct {
    Coordinates2D position;
    uint8_t red;
    uint8_t green;
    uint8_t blue;
} Pixel;

typedef struct {
    char *name;
    uint16_t width;
    uint16_t height;
    uint8_t *subpixels;
} Image;

Image get_image(char *name, uint16_t width, uint16_t height);
void fill_pixel_on_image(Image *image_ptr, Pixel pixel);
void render_image(Image image);
void free_image(Image *image_ptr);

#endif