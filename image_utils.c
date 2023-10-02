#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "stb_image_write.h"
#include "image_utils.h"

/**
 * Returns an image structure with newly allocated subpixel array
*/
Image get_image(char *name, uint16_t width, uint16_t height) {
    Image image;
    image.name = name;
    image.width = width;
    image.height = height;
    image.subpixels = (uint8_t *)malloc(sizeof(uint8_t) * width * height * PIXEL_CHANNELS);
    return image;
}

/**
 * Colors in a pixel on the subpixels array of an image structure
*/
void fill_pixel_on_image(Image *image_ptr, Pixel pixel) {
    uint32_t pixel_start_index = (pixel.position.y * image_ptr->width + pixel.position.x) * PIXEL_CHANNELS;
    image_ptr->subpixels[pixel_start_index + RED_CHANNEL_OFFSET] = pixel.red;
    image_ptr->subpixels[pixel_start_index + GREEN_CHANNEL_OFFSET] = pixel.green;
    image_ptr->subpixels[pixel_start_index + BLUE_CHANNEL_OFFSET] = pixel.blue;
}

/**
 * Renders an image using stb_image_write library with given image structure
*/
void render_image(Image image) {
    stbi_write_png(image.name, image.width, image.height, PIXEL_CHANNELS, image.subpixels, image.width * PIXEL_CHANNELS);
}

/**
 * Frees allocated subpixels array in an image structure
*/
void free_image(Image *image_ptr) {
    free(image_ptr->subpixels);
}