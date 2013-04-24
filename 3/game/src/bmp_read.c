#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "bmp_read.h"
#include "utils.h"

bitmap_t* eeds_create_bitmap(int w, int h){
    bitmap_t *bitmap = malloc(sizeof(*bitmap));
    bitmap->width = w;
    bitmap->height = h;
    bitmap->bitmap = malloc(sizeof(*bitmap->bitmap) * bitmap->height);
    for (int i = 0; i < bitmap->height; i++) {
        bitmap->bitmap[i] = malloc(sizeof(**bitmap->bitmap) * bitmap->width);
    }
    return bitmap;
}

bitmap_t *eeds_load_bmp(FILE *bmp) {
    int i, x, y, w, h, bytes;
    unsigned char *file_buffer;
    unsigned char header[54];
    bitmap_t* bitmap;

    fread(header, sizeof(*header), 54, bmp);

    w = FLIP_ENDIANNESS_INT32(*((int *) &(header[18])));
    h = FLIP_ENDIANNESS_INT32((*((int *) &(header[22]))));

    bitmap = eeds_create_bitmap(w, h);

    bytes = bitmap->width * bitmap->height * 4;
    file_buffer = malloc(sizeof(*file_buffer) * bytes);
    fread(file_buffer, sizeof(*file_buffer), bytes, bmp);

    for (i = 0, x = 0, y = 0; i < bytes;) {
        bitmap->bitmap[bitmap->height - 1 - x][y].blue = file_buffer[i++] & 255;
        bitmap->bitmap[bitmap->height - 1 - x][y].green = file_buffer[i++] & 255;
        bitmap->bitmap[bitmap->height - 1 - x][y].red = file_buffer[i++] & 255;
        i++;
        if (++y == bitmap->width) {
            x++;
            y = 0;
        }
    }

    free(file_buffer);

    return bitmap;
}

void eeds_free_bitmap(bitmap_t *bitmap) {
    for (int i = 0; i < bitmap->height; i++) {
        free(bitmap->bitmap[i]);
    }

    free(bitmap->bitmap);
    free(bitmap);
}


void eeds_render_bitmap(bitmap_t* source, bitmap_t* destination, int x, int y){
    eeds_blit(source, destination, x, y, 0, 0, source->width, source->height);
}
