#ifndef BMP_READ_H
#define BMP_READ_H

#include <stdio.h>

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

bitmap_t *load_bmp(FILE *bmp);
void free_bitmap(bitmap_t *bitmap);

#endif
