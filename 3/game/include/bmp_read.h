#ifndef BMP_READ_H
#define BMP_READ_H

#include <stdio.h>
#include <stdint.h>

typedef struct colour_struct {
    unsigned char red   : 8;
    unsigned char green : 8;
    unsigned char blue  : 8;
} colour_t;

typedef struct {
    int width;
    int height;
    colour_t **bitmap;
} bitmap_t;

bitmap_t *eeds_load_bmp(FILE *bmp);
void eeds_free_bitmap(bitmap_t *bitmap);
bitmap_t* eeds_create_bitmap(int w, int h);
void eeds_render_bitmap(bitmap_t* source, bitmap_t* destination, int x, int y);

#endif
