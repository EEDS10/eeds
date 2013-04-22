#ifndef BMP_READ_H
#define BMP_READ_H

#include <stdio.h>
#include <stdint.h>

typedef struct colour_struct {
    int red;
    int green;
    int blue;
} colour_t;

typedef struct bitmap_struct {
    int width;
    int height;
    colour_t **bitmap;
} bitmap_t;

bitmap_t *eeds_load_bmp(FILE *bmp);
void free_bitmap(bitmap_t *bitmap);
bitmap_t* eeds_create_bitmap(int w, int h);
void render_bitmap(bitmap_t* bitmap, uint8_t** screen, int x, int y);

#endif
